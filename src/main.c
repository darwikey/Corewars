#include "global.h"
#include "display.h"
#include "bot_loader.h"
#include "simulator.h"
#include <time.h>
#include <string.h>


int main(int argc, char** argv)
{
    init_display();

    srand(time(NULL));

    int _score_bot1 = 0;
    int _score_bot2 = 0;

    bool _step = false;

    while (_score_bot1 != 5 && _score_bot2 != 5)
    {
        // fill the core with instruction DAT 0
        core_cell _core[SIZE_CORE];

        for (int i = 0; i < SIZE_CORE; i++)
        {
            _core[i].instruction = 0;
            _core[i].modifier = 0;
        }

        // Loading bots
        bot _bot1;
        bot _bot2;

        initialize_bot(&_bot1, 1);
        initialize_bot(&_bot2, 2);

	if (argc == 4)
	{
	    if (strcmp("step", argv[3]) == 0)
	    {
		_step = true;
	    }
        }

        if (argc >= 3)
        {
            load_bot(argv[1], &_bot1, _core);
            load_bot(argv[2], &_bot2, _core);
        }
        else
        {
            // delete window
            endwin();

            printf("Il manque des arguments\n");

            return EXIT_FAILURE;
        }


        // Display
        for (int i = 0; i < 10000; i++)
        {
            clear();

            display_core(_core);

            display_bot(&_bot1, 0);
            display_bot(&_bot2, 10);

            simulate_bot(&_bot1, _core);
            simulate_bot(&_bot2, _core);

            display_score_bot(_score_bot1, _score_bot2);

            // Bot 1 is crashed
            if (_bot1.list_process == NULL)
            {
                _score_bot2 += 1;
                break;
            }
            // Bot 2 is crashed
            else if (_bot2.list_process == NULL)
            {
                _score_bot1 += 1 ;
                break;
            }

            if (i % 50 == 0)
            {
                refresh();
            }

            // Wait for user input
            if (_step)
	    {
		getch();
	    }
	}

        display_score_bot(_score_bot1, _score_bot2);
        display_bot(&_bot1, 0);
        display_bot(&_bot2, 10);

        // Wait for user input
        getch();

    }

    if (_score_bot1 > _score_bot2)
    {
        display_winner(1);
    }
    else
    {
        display_winner(2);
    }

    // Wait for user input
    getch();

    // delete window
    endwin();

    return 0;
}
