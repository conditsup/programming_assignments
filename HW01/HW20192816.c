#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define LEN_MIN 15 // 기차 길이
#define LEN_MAX 50
#define STM_MIN 0 // M 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90
#define AGGRO_MIN 1 // 어그로 범위
#define AGGRO_MAX 5 
#define MOVE_LEFT 1 // M의 이동 방향
#define MOVE_STAY 0 
#define ATK_NONE 0 // 좀비의 공격 대상
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
#define ACTION_REST 0  // M의 행동
#define ACTION_PROVOKE 1
#define ACTION_PULL 3

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
    int zStunned = 0;

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

    // 초기 상태 출력
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n");

    while (1) {
        // 1페이즈: 이동
        int cMoveProb = 100 - PROB;
        int zMoveProb = PROB;
        int cPrevious = cPosition;
        int zPrevious = zPosition;
        int mPrevious = mPosition;
        int mAction;

        // Citizen의 이동
        moveProb = rand() % 100;
        if (moveProb < cMoveProb) {
            cPosition--;
            cAggro++;
        }
        else {
            cAggro--;
        }
        printf("Citizen: %d -> %d (aggro: %d)\n", cPrevious, cPosition, cAggro);

        // AGGRO 수치 조정
        if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
        if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
        if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;
        if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;

        // Zombie의 이동
        if (!zStunned) {
            if (zTurnCount % 2 == 0) {
                if (moveProb < zMoveProb) {
                    int previousZPosition = zPosition;
                    if (cAggro >= mAggro && abs(cPosition - zPosition) > 1) {
                        zPosition--;
                    }
                    else if (mAggro > cAggro && abs(mPosition - zPosition) > 1) {
                        zPosition++;
                    }
                    printf("Zombie: %d -> %d\n", previousZPosition, zPosition);
                }
                else {
                    printf("Zombie: stay %d\n", zPosition);
                }
            }
            else {
                moveProb = rand() % 100;
                if (moveProb < zMoveProb) {
                    int previousZPosition = zPosition;
                    if (cAggro >= mAggro && abs(cPosition - zPosition) > 1) {
                        zPosition--;
                    }
                    else if (mAggro > cAggro && abs(mPosition - zPosition) > 1) {
                        zPosition++;
                    }
                    printf("Zombie: %d -> %d\n", previousZPosition, zPosition);
                }
                else {
                    printf("Zombie: stay (cannot move) %d\n", zPosition);
                }
            }
        }
        else {
            zStunned = 0;
        }

        printf("\n");

        // 열차 상태 출력 (이동 후)
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);


        // M의 이동 선택 및 상태 출력
        printf("M's movement ");
        if (abs(mPosition - zPosition) == 1) {
            printf("(0: stay) >> ");
        }
        else {
            printf("(0: stay, 1: move left) >> ");
        }
        if (scanf_s("%d", &mAction) != 1 || (mAction != MOVE_STAY && mAction != MOVE_LEFT)) {
            printf("Invalid input. Exit the program..\n");
            return 1;
        }
        if (mAction == MOVE_STAY) {
            printf("M stays (aggro: %d, stamina: %d)\n", mAggro, STM);
        }
        else if (mAction == MOVE_LEFT) {
            mPosition--;
            mAggro++;
            STM--;
            printf("M: %d -> %d (aggro: %d, stamina: %d)\n", mPrevious, mPosition, mAggro, STM);
        }




        // 열차 상태 출력 (이동 후)
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);

        // 2페이즈: 행동
        // Citizen의 행동 /승리 조건임
        printf("Citizen does nothing\n");
        if (cPosition == 1) {
            printf("YOU WIN!\n");
            break;
        }

        // Zombie의 행동
        if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
            if (cAggro >= mAggro) {
                printf("Zombie attacks citizen!\nGAME OVER!\n");
                break;
            }
            else {
                STM--;
                printf("Zombie attacks M! (stamina: %d)\n", STM);
                if (STM == STM_MIN) {
                    printf("GAME OVER M!\n");
                    break;
                }
            }
        }
        else if (abs(zPosition - cPosition) == 1) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            break;
        }
        else if (abs(zPosition - mPosition) == 1) {
            STM--;
            printf("Zombie attacks M! (stamina: %d)\n", STM);
            if (STM == STM_MIN) {
                printf("GAME OVER M!\n");
                break;
            }
        }
        else {
            printf("Zombie does nothing\n");
        }

        // M의 행동 선택 및 상태 출력
        printf("M's action (0: rest, 1: provoke");
        if (abs(mPosition - zPosition) <= 1) {
            printf(", 3: pull");
        }
        printf(") >> ");
        if (scanf_s("%d", &mAction) != 1 || (mAction != ACTION_REST && mAction != ACTION_PROVOKE && (mAction != ACTION_PULL || abs(mPosition - zPosition) > 1))) {
            printf("Invalid input. Exit the program..\n");
            return 1;
        }

        printf("\n");

        // M의 행동
        if (mAction == ACTION_REST) {
            STM++;
            mAggro--;
            printf("M rests (aggro: %d, stamina: %d)\n", mAggro, STM);
        }
        else if (mAction == ACTION_PROVOKE) {
            mAggro = AGGRO_MAX;
            printf("M provokes (aggro: %d, stamina: %d)\n", mAggro, STM);
        }
        else if (mAction == ACTION_PULL && abs(mPosition - zPosition) <= 1) {
            mAggro += 2;
            STM--;
            moveProb = rand() % 100;
            if (moveProb >= PROB) {
                zStunned = 1;
                printf("M pulls success (aggro: %d, stamina: %d)\n", mAggro, STM);
            }
            else {
                printf("M pulls failed (aggro: %d, stamina: %d)\n", mAggro, STM);
            }
        }


        printf("\n\n");
        }

        return 0;
    }

