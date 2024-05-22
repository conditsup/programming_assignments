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
#define MOVE_LEFT 1 // 이동 방향
#define MOVE_STAY 0 
#define ACTION_REST 0  // 행동
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int LEN, PROB, STM;
int cPosition, zPosition, mPosition;
int cAggro = 1, mAggro = 1;
int zTurnCount = 0;
int zStunned = 0;

// 기차 상태 출력 함수
void printPattern(int length) {
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n");
}

// 기차와 캐릭터 출력 함수
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

// 게임 초기화 함수
void initializeGame() {
    srand((unsigned int)time(NULL));

    // 입력값이 범위를 벗어날 때까지 반복하여 입력 받음
    while (1) {
        printf("train length(%d~%d) >> ", LEN_MIN, LEN_MAX);
        if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {
            printf("Invalid input. Please enter a number between %d and %d.\n", LEN_MIN, LEN_MAX);
            fflush(stdin); // 입력 버퍼를 비움
        }
        else {
            break;
        }
    }

    // 나머지 입력값도 동일하게 처리
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

    cAggro = 1; // 시민의 aggro 기본값
    mAggro = 1; // M의 aggro 기본값

    printf("\n\n");
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n\n");
}

// 시민 이동 함수
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

// 좀비 이동 함수
void moveZombie() {
    if (!zStunned) {
        if (zTurnCount % 2 == 0) {
            if (abs(cPosition - zPosition) == 1) {
                return; // 시민과의 거리가 1이면 움직일 수 없음
            }
            int moveProb = rand() % 100;
            if (moveProb < PROB && abs(cPosition - zPosition) > 1) {
                if (cAggro >= mAggro) {
                    zPosition--; // 시민이 더 높은 aggro를 가질 때 왼쪽으로 이동
                }
                else if (mAggro > cAggro && abs(mPosition - zPosition) > 1) {
                    zPosition++; // M이 더 높은 aggro를 가지고 거리가 1 이상일때 오른쪽으로 이동
                }
            }
        }
        else {
            if (abs(cPosition - zPosition) == 1) {
                return; // 시민과의 거리가 1이면 움직일 수 없음
            }
            int moveProb = rand() % 100;
            if (moveProb < PROB && abs(cPosition - zPosition) > 1) {
                if (cAggro >= mAggro) {
                    zPosition--; // 시민이 더 높은 aggro를 가질 때 왼쪽으로 이동
                }
                else if (mAggro > cAggro && abs(mPosition - zPosition) > 1) {
                    zPosition++; // M이 더 높은 aggro를 가지고 거리가 1 이상일 때 오른쪽으로 이동
                }
            }
        }
    }
}

// M 이동 함수
void moveM() {
    int mAction;

    // 좀비와 붙어있을 경우
    if (abs(mPosition - zPosition) == 1) {
        // 이동하지 않도록 선택지 0번만 제공
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
        // 좀비와 붙어있지 않을 경우에만 이동할 방향을 선택하도록 요청
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

        // 선택에 따라 위치 변경
        if (mAction == MOVE_LEFT && mPosition > 0) {
            mPosition--;
        }
    }
}






// 시민 행동 처리 함수
void actionC() {
    printf("Citizen does nothing\n");
}

// 좀비 행동 처리 함수
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

// M 행동 처리 함수
void actionM() {
    int mAction;

    // 붙어있는 경우 이동하지 않고 휴식만 선택 가능
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



// 시민 이동 결과 출력 함수
void printCitizenMoveResult() {
    if (cPosition != cPosition + 1) {
        printf("Citizen: %d -> %d (aggro: %d)\n", cPosition + 1, cPosition, cAggro);
    } else {
        printf("Citizen: stay %d (aggro: %d)\n", cPosition, cAggro);
    }
}

// 좀비 이동 결과 출력 함수
void printZombieMoveResult() {
    if (zPosition != zPosition + 1) {
        printf("Zombie: %d -> %d\n", zPosition + 1, zPosition);
    } else {
        printf("Zombie: stay %d\n", zPosition);
    }
}

// M 이동 결과 출력 함수
void printMMoveResult(int mAction) {
    if (mAction == MOVE_LEFT && mPosition > 0) {
        printf("M: %d -> %d (aggro: %d, stamina: %d)\n", mPosition + 1, mPosition, mAggro, STM);
    }
    else {
        printf("M stays %d (aggro: %d, stamina: %d)\n", mPosition, mAggro, STM);
    }
}







// 메인 함수
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


        // 이동 결과 출력
        printf("\n");

        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);

        actionC();
        actionZ();
        actionM();
    }

    return 0;
}