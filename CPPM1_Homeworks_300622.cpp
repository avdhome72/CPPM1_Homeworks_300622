#include <iostream>
#include <string>
#include <sstream>

using UI = unsigned int;

class GeomException : public std::exception
{
public:
    GeomException(const std::string& message) : std::exception(message.c_str())
    {
    }
};

class Figure
{
public:
    Figure(const std::string &name = "Фигура", UI nSides = 0) : m_name(name), m_nSides(nSides)
    {
    }
    virtual ~Figure() = default;

    virtual std::string printInfo() const
    {
        std::stringstream os;
        os << "\n" << m_name << ":\n";
        os << (check() ? "Правильная" : "Неправильная") << "\n";
        os << "Сторон: " << m_nSides << "\n";

        return os.str();
    }

    virtual bool check() const
    {
        return m_nSides == 0;
    }

protected:
    std::string m_name;
    UI m_nSides;
};

class Triangle : public Figure
{
public:
    Triangle(UI a, UI b, UI c, UI A, UI B, UI C, const std::string& name = "Треугольник")
        : Figure(name, 3), m_a(a), m_b(b), m_c(c), m_A(A), m_B(B), m_C(C)
    {
        if (m_nSides != 3)
        {
            throw GeomException(printInfo() + " - Неверное число сторон");
        }
        if (m_A + m_B + m_C != 180)
        {
            throw GeomException(printInfo() + " - Сумма углов не равна 180");
        }
        if (m_a + m_b <= m_c)
        {
            throw GeomException(printInfo() + " - Сумма длин двух сторон менее или равна третьей");
        }
    }

    bool operator == (const Triangle &other) const
    {
        return m_a == other.m_a && m_b == other.m_b && m_c == other.m_c &&
            m_A == other.m_A && m_B == other.m_B && m_C == other.m_C;
    }

    UI perimeter() const
    {
        return m_a + m_b + m_c;
    }

    UI operator +(const Triangle& other) const
    {
        return perimeter() + other.perimeter();
    }

    Triangle& operator ++()
    {
        m_a *= 2;
        m_b *= 2;
        m_c *= 2;

        return *this;
    }

    Triangle& operator ++(int)
    {
        return ++ * this;
    }

    operator int() const
    {
        return perimeter();
    }

    std::string printInfo() const override
    {
        std::stringstream os;

        os << Figure::printInfo();
        os << "Стороны: " << m_a << ", " << m_b << ", " << m_c << "\n";
        os << "Углы: " << m_A << ", " << m_B << ", " << m_C << "\n";

        return os.str();
    }

    bool check() const override
    {
        return m_nSides == 3 && (m_A + m_B + m_C == 180);
    }

protected:
    UI m_a, m_b, m_c;
    UI m_A, m_B, m_C;
};

class Triangle90 : public Triangle
{
public:
    Triangle90(UI a, UI b, UI c, UI A, UI B) : Triangle(a, b, c, A, B, 90, "Прямоугольный треугольник")
    {
        if (m_C != 90)
        {
            throw GeomException(printInfo() + " - Угол C не равен 90");
        }
    }

    bool check() const override
    {
        return Triangle::check() && m_C == 90;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    const Figure f;
    const Triangle t(3, 4, 5, 50, 100, 30);
    const Triangle t2(3, 4, 5, 50, 40, 90);
    Triangle90 t90(33, 44, 55, 50, 40);

    std::cout << f.printInfo();
    std::cout << t.printInfo();
    std::cout << t90.printInfo();
    
    std::cout << std::boolalpha << std::endl << (t90 == t2) << std:: endl;
    std::cout << std::endl << (t90 + t2) << std::endl;
    std::cout << std::endl << (t90++) << std::endl;
    std::cout << std::endl << (++t90) << std::endl;

    try
    {
        Triangle goodTriangle(6, 7, 12, 40, 20, 120);
        std::cout << goodTriangle.printInfo() << " - создан.";

        Triangle90 wrongTriangle(6, 1, 12, 40, 50);
        std::cout << wrongTriangle.printInfo();
    }
    catch (const GeomException &e)
    {
        std::cout << e.what() << " - не создан!";
    }

}
