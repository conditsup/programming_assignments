#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define LEN_MIN 15 // ���� ����
#define LEN_MAX 50
#define STM_MIN 0 // M ü��
#define STM_MAX 5
#define PROB_MIN 10 // Ȯ��
#define PROB_MAX 90
#define AGGRO_MIN 1 // ��׷� ����
#define AGGRO_MAX 5 
#define MOVE_LEFT 1 // M�� �̵� ����
#define MOVE_STAY 0 
#define ATK_NONE 0 // ������ ���� ���
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
#define ACTION_REST 0  // M�� �ൿ
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

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

    int LEN, PROB, moveProb, STM = 0;
    int cPosition, zPosition, mPosition;
    int zTurnCount = 0;
    int cAggro = 1, mAggro = 1;

    printf("train length(%d~%d) >> ", LEN_MIN, LEN_MAX);
    if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {
        printf("Invalid input. Exit the program..\n");
        return 1;
    }

    printf("M stamina(0~5)>> ");
    if (scanf_s("%d", &STM) != 1 || STM < STM_MIN || STM > STM_MAX) {
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
        int mPrevious = mPosition;
        int mAction;

        // Citizen�� �̵�
        moveProb = rand() % 100;
        if (moveProb < cMoveProb) {
            cPosition--;
            cAggro++;
        }
        else {
            cAggro--;
        }

        // Zombie�� �̵� 
        moveProb = rand() % 100;
        if (zTurnCount % 2 == 0 && moveProb < zMoveProb) {
            zPosition--;
        }
        zTurnCount++;

        // AGGRO ��ġ ����
        if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
        if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
        if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;
        if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;

        // ���� ���� ���
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);

        // c���� ���
        if (cPosition == 1) {
            printf("SUCCESS!\ncitizen(s) escaped to the next train\n");
            break;
        }
        else if (cPosition != cPrevious) {
            printf("citizen: %d -> %d (aggro: %d)\n", cPrevious, cPosition, cAggro);
        }
        else {
            printf("citizen: stay %d (aggro: %d)\n", cPosition, cAggro);
        }

        // z���� ���
        if (zTurnCount % 2 == 0) {
            printf("zombie: stay %d (cannot move)\n", zPosition);
        }
        else if (zPosition != zPrevious) {
            printf("zombie: %d -> %d\n", zPrevious, zPosition);
        }
        else {
            printf("zombie: stay %d\n", zPosition);
        }

        printf("\n");

        // M�� �̵� ���� �� ���� ���
        printf("madongseok move (0: stay, 1: move left) >> ");
        if (scanf_s("%d", &mAction) != 1 || (mAction != MOVE_STAY && mAction != MOVE_LEFT)) {
            printf("Invalid input. Exit the program..\n");
            return 1;
        }
        if (mAction == MOVE_STAY) {
            printf("madongseok stay (aggro: %d, stamina: %d)\n", mAggro, STM);
            mAggro--;
        }
        else if (mAction == MOVE_LEFT) {
            mPosition--;
            mAggro++;
            STM--;
            printf("madongseok: %d -> %d (aggro: %d, stamina: %d)\n", mPrevious, mPosition, mAggro, STM);
        }

        // ���� ���� ���
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);

        // m���� ���
        if (mPosition == mPrevious) {
            printf("madongseok: stay %d (aggro: %d, stamina: %d)\n", mPosition, mAggro, STM);
        }

        printf("\n");

        // ���� ���� ����
        if (abs(cPosition - zPosition) == 1) {
            printf("\nGAME OVER!\nCitizen(s) has(have) been attacked by a zombie\n");
            break;
        }

        printf("\n\n");
    }

    return 0;
}
