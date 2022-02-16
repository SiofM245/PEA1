#include "pch.h"
#include "Menu.h"

#ifdef WIN
#include <windows.h>
#else
#include <time.h>
#endif

namespace zad
{
    using number = uint64_t;
    constexpr number MILION{ 1000000ul };
    constexpr number BILLION{ 1000ul * MILION };

#ifdef WIN

    number get_current_microseconds()
	{
		LARGE_INTEGER clock_data, freq;

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&clock_data);

		return (clock_data.QuadPart * MILION) / freq.QuadPart;
	}


#else
    number get_current_microseconds()
    {
        timespec clock_data;
        clock_gettime(CLOCK_MONOTONIC, &clock_data);
        return static_cast<number>(
                (
                        (
                                BILLION * clock_data.tv_sec
                        ) + clock_data.tv_nsec
                ) / 1000.0
        );
    }

#endif


    class timer
    {
        number m_start;
        number m_stop;

    public:

        void reset()
        {
            m_start = 0;
            m_stop = 0;
        }

        void start() { m_start = get_current_microseconds(); }
        void stop() { m_stop = get_current_microseconds(); }
        number get_time() const { return  m_stop - m_start; }
    };

    struct scoped_timer : timer
    {
        scoped_timer() { start(); }
    };

}


void Menu::enable()
{
    zad::scoped_timer timer;
    timer.stop();
    timer.reset();

	Graph graph;

	while (true)
	{
		int select = 0;
		string pathName = "";
		bool check = false;

		cout << preview;
		cout << options;

		cin >> select;


		switch (select)
		{
		case 1:
		{
			do
			{
				cout << "Wpisz nazwe pliku ";
				cin >> pathName;
				check = graph.readGraph(pathName);

				if (!check)
				{
					cout << "Blad odczytu\n";
				}

			} while (!check);


			preview = "Plik <" + pathName + ">\n\n";
			preview.append(graph.toString());
			graph.infDiag();

			break;
		}

		case 2:
		{
			if (!graph.getCount())
			{
				cout << "Brak danych\n";
				cin.ignore();
				cin.get();
			}
			else
			{
				TSP_BB bb(graph);
				timer.reset();
				timer.start();
				bb.apply();
				timer.stop();

				cout<< "Czas trwania programu - "<<timer.get_time()<<" nanosekund ("<<timer.get_time()/1000000000<<" s)\n";
				cout << "-----------Branch & Bound----------\n" + bb.toString();

				cin.ignore();
				cin.get();
			}
			break;
		}

		case 3:
		{
			if (!graph.getCount())
			{
				cout << "Brak danych\n";
				cin.ignore();
				cin.get();
			}
			else
			{
				TSP_DP dp(graph);
				timer.reset();
                timer.start();
				dp.apply();
				timer.stop();
				cout<< "Czas trwania programu - "<<timer.get_time()<<" nanosekund ("<<timer.get_time()/1000000000<<" s)\n";
				cout << "---------Dynamic Programming-------\n" + dp.toString();

				cin.ignore();
				cin.get();
			}

			break;
		}

		case 4:
		{
			return;
		}
		}

		system("cls");
	}
}

Menu::Menu()
{
	preview = "";
	options = "";
	options.append("-----------------------------------\n");
	options.append("1. Wczytywanie grafu\n");
	options.append("2. Algorytm podzialu i ograniczen\n");
	options.append("3. Algorytm dynamicznego porogramowania\n");
	options.append("4. Wyjscie\n");
	options.append("Wybierz opcje: ");
}


Menu::~Menu()
{
}
