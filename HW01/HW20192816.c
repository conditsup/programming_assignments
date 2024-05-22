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
#define MOVE_LEFT 1 // �̵� ����
#define MOVE_STAY 0 
#define ACTION_REST 0  // �ൿ
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int LEN, PROB, STM;
int cPosition, zPosition, mPosition;
int cAggro = 1, mAggro = 1;
int zTurnCount = 0;
int zStunned = 0;

// ���� ���� ��� �Լ�
void printPattern(int length) {
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n");
}

// ������ ĳ���� ��� �Լ�
void printPatternWithCharacters(int length, int cPos, int zPos, int mPos) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == cPos) {
            printf("C");
        }
        else if (i == zPos) {
            printf("Z");
        }
        else if (i == mPos) {
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

// ���� �ʱ�ȭ �Լ�
void initializeGame() {
    srand((unsigned int)time(NULL));

    // �Է°��� ������ ��� ������ �ݺ��Ͽ� �Է� ����
    while (1) {
        printf("train length(%d~%d) >> ", LEN_MIN, LEN_MAX);
        if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {
            printf("Invalid input. Please enter a number between %d and %d.\n", LEN_MIN, LEN_MAX);
            fflush(stdin); // �Է� ���۸� ���
        }
        else {
            break;
        }
    }

    // ������ �Է°��� �����ϰ� ó��
    while (1) {
        printf("M stamina(%d~%d)>> ", STM_MIN, STM_MAX);
        if (scanf_s("%d", &STM) != 1 || STM < STM_MIN || STM > STM_MAX) {
            printf("Invalid input. Please enter a number between %d and %d.\n", STM_MIN, STM_MAX);
            fflush(stdin);
        }
        else {
            break;
        }
    }

    while (1) {
        printf("percentile probability 'p'(%d~%d) >> ", PROB_MIN, PROB_MAX);
        if (scanf_s("%d", &PROB) != 1 || PROB < PROB_MIN || PROB > PROB_MAX) {
            printf("Invalid input. Please enter a number between %d and %d.\n", PROB_MIN, PROB_MAX);
            fflush(stdin);
        }
        else {
            break;
        }
    }

    cPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    cAggro = 1; // �ù��� aggro �⺻��
    mAggro = 1; // M�� aggro �⺻��

    printf("\n\n");
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n\n");
}

// �ù� �̵� �Լ�
void moveCitizen() {
    int cMoveProb = 100 - PROB;
    int moveProb = rand() % 100;
    if (moveProb < cMoveProb) {
        cPosition--;
        cAggro++;
    }
    else {
        cAggro--;
    }

    if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
    if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
}

// ���� �̵� �Լ�
void moveZombie() {
    if (!zStunned) {
        if (zTurnCount % 2 == 0) {
            if (abs(cPosition - zPosition) == 1) {
                return; // �ùΰ��� �Ÿ��� 1�̸� ������ �� ����
            }
            int moveProb = rand() % 100;
            if (moveProb < PROB && abs(cPosition - zPosition) > 1) {
                if (cAggro >= mAggro) {
                    zPosition--; // �ù��� �� ���� aggro�� ���� �� �������� �̵�
                }
                else if (mAggro > cAggro && abs(mPosition - zPosition) > 1) {
                    zPosition++; // M�� �� ���� aggro�� ������ �Ÿ��� 1 �̻��϶� ���������� �̵�
                }
            }
        }
        else {
            if (abs(cPosition - zPosition) == 1) {
                return; // �ùΰ��� �Ÿ��� 1�̸� ������ �� ����
            }
            int moveProb = rand() % 100;
            if (moveProb < PROB && abs(cPosition - zPosition) > 1) {
                if (cAggro >= mAggro) {
                    zPosition--; // �ù��� �� ���� aggro�� ���� �� �������� �̵�
                }
                else if (mAggro > cAggro && abs(mPosition - zPosition) > 1) {
                    zPosition++; // M�� �� ���� aggro�� ������ �Ÿ��� 1 �̻��� �� ���������� �̵�
                }
            }
        }
    }
}

// M �̵� �Լ�
void moveM() {
    int mAction;

    // ����� �پ����� ���
    if (abs(mPosition - zPosition) == 1) {
        // �̵����� �ʵ��� ������ 0���� ����
        printf("M's movement (0: stay) >> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || mAction != MOVE_STAY) {
                printf("Invalid input. Please enter 0.\n");
                fflush(stdin);
            }
            else {
                break;
            }
        }
    }
    else {
        // ����� �پ����� ���� ��쿡�� �̵��� ������ �����ϵ��� ��û
        printf("M's movement (0: stay, 1: move left) >> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || (mAction != MOVE_STAY && mAction != MOVE_LEFT)) {
                printf("Invalid input. Please enter 0 or 1.\n");
                fflush(stdin);
            }
            else {
                break;
            }
        }

        // ���ÿ� ���� ��ġ ����
        if (mAction == MOVE_LEFT && mPosition > 0) {
            mPosition--;
        }
    }
}






// �ù� �ൿ ó�� �Լ�
void actionC() {
    printf("Citizen does nothing\n");
}

// ���� �ൿ ó�� �Լ�
void actionZ() {
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            exit(1);
        }
        else {
            STM--;
            printf("Zombie attacks M! (stamina: %d)\n", STM);
            if (STM == STM_MIN) {
                printf("GAME OVER M!\n");
                exit(1);
            }
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        printf("Zombie attacks citizen!\nGAME OVER!\n");
        exit(1);
    }
    else if (abs(zPosition - mPosition) == 1) {
        STM--;
        printf("Zombie attacks M! (stamina: %d)\n", STM);
        if (STM == STM_MIN) {
            printf("GAME OVER M!\n");
            exit(1);
        }
    }
    else {
        printf("Zombie does nothing\n");
    }
}

// M �ൿ ó�� �Լ�
void actionM() {
    int mAction;

    // �پ��ִ� ��� �̵����� �ʰ� �޽ĸ� ���� ����
    if (abs(mPosition - zPosition) == 1) {
        mAction = ACTION_REST;
    }
    else {
        while (1) {
            printf("M's action (0: rest");
            if (abs(mPosition - zPosition) <= 1) {
                printf(", 1: provoke");
            }
            printf(") >> ");

            if (scanf_s("%d", &mAction) != 1 || (mAction != ACTION_REST && (mAction != ACTION_PROVOKE || abs(mPosition - zPosition) > 1))) {
                printf("Invalid input. ");
                if (abs(mPosition - zPosition) <= 1) {
                    printf("Please enter 0 or 1.\n");
                }
                else {
                    printf("Please enter 0.\n");
                }
                fflush(stdin);
            }
            else {
                break;
            }
        }
    }

    if (mAction == ACTION_REST) {
        STM++;
        mAggro--;
        printf("M rests (aggro: %d, stamina: %d)\n", mAggro, STM);
    }
    else if (mAction == ACTION_PROVOKE) {
        mAggro = AGGRO_MAX;
        printf("M provokes (aggro: %d, stamina: %d)\n", mAggro, STM);
    }
}



// �ù� �̵� ��� ��� �Լ�
void printCitizenMoveResult() {
    if (cPosition != cPosition + 1) {
        printf("Citizen: %d -> %d (aggro: %d)\n", cPosition + 1, cPosition, cAggro);
    } else {
        printf("Citizen: stay %d (aggro: %d)\n", cPosition, cAggro);
    }
}

// ���� �̵� ��� ��� �Լ�
void printZombieMoveResult() {
    if (zPosition != zPosition + 1) {
        printf("Zombie: %d -> %d\n", zPosition + 1, zPosition);
    } else {
        printf("Zombie: stay %d\n", zPosition);
    }
}

// M �̵� ��� ��� �Լ�
void printMMoveResult(int mAction) {
    if (mAction == MOVE_LEFT && mPosition > 0) {
        printf("M: %d -> %d (aggro: %d, stamina: %d)\n", mPosition + 1, mPosition, mAggro, STM);
    }
    else {
        printf("M stays %d (aggro: %d, stamina: %d)\n", mPosition, mAggro, STM);
    }
}







// ���� �Լ�
int main(void) {
    initializeGame();

    while (1) {
        moveCitizen();
        moveZombie();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printCitizenMoveResult();
        printZombieMoveResult();

        printf("\n");
        moveM();


        // �̵� ��� ���
        printf("\n");

        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);

        actionC();
        actionZ();
        actionM();
    }

    return 0;
}