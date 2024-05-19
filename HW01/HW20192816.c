#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define PROB_MIN 10
#define PROB_MAX 90

void printPattern(int length) {
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n");
}


void printPatternWithCharacters(int length, int cPosition, int zPosition, int mPosition) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == cPosition) {
            printf("C");
        }
        else if (i == zPosition) {
            printf("Z");
        }
        else if (i == mPosition) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}


int main(void) {
    srand((unsigned int)time(NULL));

    int LEN, PROB, moveProb = 0;
    int cPosition, zPosition, mPosition;
    int zMoveFlag = 0;
    int zTurnCount = 0;



    //��Ʈ��
    printf("����� ������ ����մϴ�...\n\n");
    printf("                                             --         .--         --,\n");
    printf("       @@=       *@@.          ..      -@@,        ~@@.    @@@ @@#\n");
    printf("       @@$       *@@.         :@@-     -@@,        ~@@.    @@@ @@#\n");
    printf("       @@#,,,,,,,=@@.         :@@-     -@@,    @@@@@@@@@@@ @@@ @@#\n");
    printf("       @@@@@@@@@@@@@.         :@@-     -@@,    @@@@@@@@@@@ @@@ @@#\n");
    printf("       @@@@@@@@@@@@@.         *@@!     -@@,        ~::,    @@@ @@#\n");
    printf("       @@$       *@@.         $@@$     -@@-      ;@@@@@#.  @@@ @@#\n");
    printf("       @@$       *@@.         @@@@,    -@@@@@=  *@@@##@@@..@@@ @@#\n");
    printf("       @@@@@@@@@@@@@.        @@@@@@    -@@@@@=  #@#   ,@@@@@@@ @@#\n");
    printf("       @@@@@@@@@@@@@.       $@@:.@@@:  -@@:--,  @@;    @@@@@@@ @@#\n");
    printf("       -------------      ,#@@!  -@@@#.-@@,     $@@; ,$@@- @@@ @@#\n");
    printf("                         -@@@*    .#@* -@@,     -@@@@@@@$  @@@ @@#\n");
    printf("    @@@@@@@@@@@@@@@@@@@.  -@*       !  -@@,      .=#@@$;   === @@#\n");
    printf("    @@@@@@@@@@@@@@@@@@@.      ..       -@@,            .~=$$;. \n");
    printf("    ........=@#........      !@@-      -@@,          !@@@@@@@@@$\n");
    printf("            *@#              !@@-      ,$$,         $@@@#*:;$@@@@\n");
    printf("            *@#              !@@-                  ,@@@.      !@@!\n");
    printf("            *@#              !@@-                  ~@@,        @@@\n");
    printf("            *@#              !@@;~~~~~~~~~~        .@@@-     ,*@@:\n");
    printf("            *@#              !@@@@@@@@@@@@@         -@@@@@@@@@@@$\n");
    printf("            *@#              !@@@@@@@@@@@@@          .@@@@@@@@@;\n");
    printf("            ,-,                                        .-----,\n");
    printf("\n\n\n");
    printf("                                                               START...  \n\n");

    Sleep(2000);
    system("cls"); 



    printf("train length(%d~%d) >> ", LEN_MIN, LEN_MAX);
    if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {
        printf("Invalid input. Exit the program..\n");
        return 1;
    }

    printf("percentile probability 'p'(%d~%d) >> ", PROB_MIN, PROB_MAX);
    if (scanf_s("%d", &PROB) != 1 || PROB < PROB_MIN || PROB > PROB_MAX) {
        printf("Invalid input. Exit the program..\n");
        return 1;
    }

    cPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    printf("\n");


    // �ʱ� ���� ���
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n");


    while (1) {
        int cMoveProb = 100 - PROB;
        int zMoveProb = PROB;
        int cPrevious = cPosition;
        int zPrevious = zPosition;

        moveProb = rand() % 100;
        // C�� �̵�
        if (moveProb < cMoveProb) {
            cPosition--;
        }

        moveProb = rand() % 100;
        // Z�� �̵� 
        if (zTurnCount % 2 == 0 && moveProb < zMoveProb) {
            zPosition--;
        }
        zTurnCount++;

        printf("\n");


        //���� 
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);


        // ���� ���
        if (cPosition == 1) {
            printf("SUCCESS!\ncitizen(s) escaped to the next train\n");
            break;
        }
        else if (cPosition != cPrevious) {
            printf("citizen: %d -> %d\n", cPrevious, cPosition);
        }
        else {
            printf("citizen: stay %d\n", cPosition);
        }


        if (zTurnCount % 2 == 0) {
            printf("zombie: stay %d (cannot move)\n", zPosition);
        }
        else if (zPosition != zPrevious) {
            printf("zombie: %d -> %d\n", zPrevious, zPosition);
        }

        else {
            printf("zombie: stay %d\n", zPosition);
        }

        if (abs(cPosition - zPosition) == 1) {
            printf("\nGAME OVER!\nCitizen(s) has(have) been attacked by a zombie\n");
            break;

        }

        Sleep(4000);
        printf("\n\n");
    }

    return 0;
}
