#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <algorithm>

class Vertex
{
public:
    virtual ~Vertex() = default;
    virtual int num_inputs() const = 0;
    virtual void set_input(int inp_idx, double inp_val) = 0;
    virtual int num_outputs() const = 0;
    virtual double get_output(int out_idx) = 0;
    virtual void calc_value() = 0;
};

struct Edge
{
    int out_vertex_id;
    int out_port_id;
    int inp_vertex_id;
    int inp_port_id;
};

class CalcGraph : public Vertex
{
private:
    std::vector<std::unique_ptr<Vertex>> vertices;
    std::vector<Edge> edges;

    // Граф представления: вершина -> список входящих ребер
    std::unordered_map<int, std::vector<Edge>> incoming_edges;

    // Топологически отсортированный порядок вершин
    std::vector<int> topological_order;

    // Свободные входные порты графа (не подключенные к другим вершинам)
    std::vector<std::pair<int, int>> free_inputs;

    // Свободные выходные порты графа (не подключенные к другим вершинам)
    std::vector<std::pair<int, int>> free_outputs;

    // Входные значения для графа
    std::vector<double> graph_inputs;

    // Вспомогательная функция для поиска пары в векторе
    bool find_and_remove(std::vector<std::pair<int, int>> &vec, const std::pair<int, int> &value)
    {
        auto it = std::find(vec.begin(), vec.end(), value);
        if (it != vec.end())
        {
            vec.erase(it);
            return true;
        }
        return false;
    }

    void build_graph()
    {
        incoming_edges.clear();
        free_inputs.clear();
        free_outputs.clear();

        // Инициализируем все порты как свободные
        for (int i = 0; i < vertices.size(); ++i)
        {
            for (int j = 0; j < vertices[i]->num_inputs(); ++j)
            {
                free_inputs.push_back({i, j});
            }
            for (int j = 0; j < vertices[i]->num_outputs(); ++j)
            {
                free_outputs.push_back({i, j});
            }
        }

        // Обрабатываем ребра
        for (const auto &edge : edges)
        {
            // Проверяем валидность вершин и портов
            if (edge.out_vertex_id < 0 || edge.out_vertex_id >= vertices.size() ||
                edge.inp_vertex_id < 0 || edge.inp_vertex_id >= vertices.size())
            {
                throw std::out_of_range("Invalid vertex id in edge");
            }

            if (edge.out_port_id < 0 || edge.out_port_id >= vertices[edge.out_vertex_id]->num_outputs() ||
                edge.inp_port_id < 0 || edge.inp_port_id >= vertices[edge.inp_vertex_id]->num_inputs())
            {
                throw std::out_of_range("Invalid port id in edge");
            }

            // Добавляем ребро в список входящих для целевой вершины
            incoming_edges[edge.inp_vertex_id].push_back(edge);

            // Убираем подключенные порты из свободных
            find_and_remove(free_inputs, {edge.inp_vertex_id, edge.inp_port_id});
            find_and_remove(free_outputs, {edge.out_vertex_id, edge.out_port_id});
        }

        topological_sort();
    }

    void topological_sort()
    {
        std::vector<int> in_degree(vertices.size(), 0);
        std::queue<int> q;

        // Вычисляем полустепень захода для каждой вершины (сколько ребер в нее входит)
        for (const auto &edge : edges)
        {
            in_degree[edge.inp_vertex_id]++;
        }

        // Находим вершины с нулевой полустепенью захода (источники)
        for (int i = 0; i < vertices.size(); ++i)
        {
            if (in_degree[i] == 0)
            {
                q.push(i);
            }
        }

        topological_order.clear();
        while (!q.empty())
        {
            int current = q.front();
            q.pop();
            topological_order.push_back(current);

            // Для текущей вершины находим все вершины, которые от нее зависят
            // (куда ведут ее выходные порты)
            for (const auto &edge : edges)
            {
                if (edge.out_vertex_id == current)
                {
                    in_degree[edge.inp_vertex_id]--;
                    if (in_degree[edge.inp_vertex_id] == 0)
                    {
                        q.push(edge.inp_vertex_id);
                    }
                }
            }
        }

        // Проверка на ацикличность
        if (topological_order.size() != vertices.size())
        {
            throw std::runtime_error("Graph contains cycles");
        }
    }

public:
    void set_data(const std::vector<Vertex *> &vertex_ptrs, const std::vector<Edge> &new_edges)
    {
        // Очищаем предыдущие данные
        vertices.clear();
        edges = new_edges;

        // Переносим владение вершинами
        for (auto *ptr : vertex_ptrs)
        {
            vertices.emplace_back(ptr);
        }

        build_graph();
        graph_inputs.resize(free_inputs.size(), 0.0);
    }

    int num_inputs() const override
    {
        return free_inputs.size();
    }

    void set_input(int inp_idx, double inp_val) override
    {
        if (inp_idx < 0 || inp_idx >= free_inputs.size())
        {
            throw std::out_of_range("Bad inp_idx in CalcGraph::set_input");
        }
        graph_inputs[inp_idx] = inp_val;
    }

    int num_outputs() const override
    {
        return free_outputs.size();
    }

    double get_output(int out_idx) override
    {
        if (out_idx < 0 || out_idx >= free_outputs.size())
        {
            throw std::out_of_range("Bad out_idx in CalcGraph::get_output");
        }
        auto [vertex_id, port_id] = free_outputs[out_idx];
        return vertices[vertex_id]->get_output(port_id);
    }

    void calc_value() override
    {
        // Сначала сбрасываем все входы вершин (на всякий случай)
        for (auto &vertex : vertices)
        {
            for (int i = 0; i < vertex->num_inputs(); ++i)
            {
                vertex->set_input(i, 0.0);
            }
        }

        // Устанавливаем входные значения для свободных входов графа
        for (int i = 0; i < free_inputs.size(); ++i)
        {
            auto [vertex_id, port_id] = free_inputs[i];
            vertices[vertex_id]->set_input(port_id, graph_inputs[i]);
        }

        // Вычисляем вершины в топологическом порядке
        for (int vertex_id : topological_order)
        {
            // Устанавливаем значения из связанных выходов (входы, которые подключены к другим вершинам)
            if (incoming_edges.find(vertex_id) != incoming_edges.end())
            {
                for (const auto &edge : incoming_edges[vertex_id])
                {
                    double output_val = vertices[edge.out_vertex_id]->get_output(edge.out_port_id);
                    vertices[vertex_id]->set_input(edge.inp_port_id, output_val);
                }
            }

            // Вычисляем текущую вершину
            vertices[vertex_id]->calc_value();
        }
    }
};

// Пример реализации оператора сложения
class PlusOperator : public Vertex
{
    std::vector<double> m_inp_val;
    double m_out_val = 0;

public:
    PlusOperator() : m_inp_val(2, 0.0) {}

    int num_inputs() const override
    {
        return 2;
    }

    void set_input(int inp_idx, double inp_val) override
    {
        if (inp_idx < 0 || inp_idx >= 2)
        {
            throw std::out_of_range("Bad inp_idx in PlusOperator::set_input");
        }
        m_inp_val[inp_idx] = inp_val;
    }

    void calc_value() override
    {
        m_out_val = m_inp_val[0] + m_inp_val[1];
    }

    int num_outputs() const override
    {
        return 1;
    }

    double get_output(int out_idx) override
    {
        if (out_idx != 0)
        {
            throw std::out_of_range("Bad out_idx in PlusOperator::get_output");
        }
        return m_out_val;
    }
};

// Пример реализации оператора умножения
class MultiplyOperator : public Vertex
{
    std::vector<double> m_inp_val;
    double m_out_val = 0;

public:
    MultiplyOperator() : m_inp_val(2, 0.0) {}

    int num_inputs() const override
    {
        return 2;
    }

    void set_input(int inp_idx, double inp_val) override
    {
        if (inp_idx < 0 || inp_idx >= 2)
        {
            throw std::out_of_range("Bad inp_idx in MultiplyOperator::set_input");
        }
        m_inp_val[inp_idx] = inp_val;
    }

    void calc_value() override
    {
        m_out_val = m_inp_val[0] * m_inp_val[1];
    }

    int num_outputs() const override
    {
        return 1;
    }

    double get_output(int out_idx) override
    {
        if (out_idx != 0)
        {
            throw std::out_of_range("Bad out_idx in MultiplyOperator::get_output");
        }
        return m_out_val;
    }
};

// Демонстрация использования
int main()
{
    // Пример 1: Простое сложение (1 + 2) + 3 = 6
    CalcGraph cg1;
    std::vector<Vertex *> vertex1;
    std::vector<Edge> edges1;

    vertex1.push_back(new PlusOperator); // vertex_id = 0
    vertex1.push_back(new PlusOperator); // vertex_id = 1

    // Подключаем выход первого плюса ко входу 0 второго плюса
    edges1.push_back(Edge{0, 0, 1, 0});

    cg1.set_data(vertex1, edges1);

    std::cout << "Graph 1 inputs: " << cg1.num_inputs() << std::endl;   // Должно быть 3
    std::cout << "Graph 1 outputs: " << cg1.num_outputs() << std::endl; // Должно быть 1

    // Устанавливаем входы: первый плюс получает (1, 2), второй плюс получает (результат первого, 3)
    cg1.set_input(0, 1.0); // Вход 0 первого плюса
    cg1.set_input(1, 2.0); // Вход 1 первого плюса
    cg1.set_input(2, 3.0); // Вход 1 второго плюса (вход 0 подключен к выходу первого)

    cg1.calc_value();

    std::cout << "Graph 1 output: " << cg1.get_output(0) << std::endl; // Должно быть 6.0

    // Пример 2: Более сложный граф (5 + 3) * 2 = 16
    CalcGraph cg2;
    std::vector<Vertex *> vertex2;
    std::vector<Edge> edges2;

    vertex2.push_back(new PlusOperator);     // vertex_id = 0
    vertex2.push_back(new MultiplyOperator); // vertex_id = 1

    // Подключаем выход сложения ко входу 0 умножения
    edges2.push_back(Edge{0, 0, 1, 0});

    cg2.set_data(vertex2, edges2);

    std::cout << "\nGraph 2 inputs: " << cg2.num_inputs() << std::endl; // Должно быть 3
    std::cout << "Graph 2 outputs: " << cg2.num_outputs() << std::endl; // Должно быть 1

    cg2.set_input(0, 5.0); // Вход 0 плюса (a)
    cg2.set_input(1, 3.0); // Вход 1 плюса (b)
    cg2.set_input(2, 2.0); // Вход 1 умножения (c)
    cg2.calc_value();

    std::cout << "Graph 2 output: " << cg2.get_output(0) << std::endl; // Должно быть 16.0

    return 0;
}