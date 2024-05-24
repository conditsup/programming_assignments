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
int mAction ;  // M�� �ൿ�� ������ ����
int zAction ;
int prevCPosition;
int prevZPosition; // ���� ������ ��ġ�� �����ϴ� ����


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

// �Է� ���۸� ���� �Լ�
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// ���� �ʱ�ȭ �Լ�
void initializeGame() {

    srand((unsigned int)time(NULL));

    // �Է°��� ������ ��� ������ �ݺ��Ͽ� �Է� ���� 
    while (1) {
        printf("train length(%d~%d) >> ", LEN_MIN, LEN_MAX);
        if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {
            printf("Invalid input. Please enter a number between %d and %d.\n", LEN_MIN, LEN_MAX);
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    // ������ �Է°��� �����ϰ� ó��
    while (1) {
        printf("M stamina(%d~%d)>> ", STM_MIN, STM_MAX);
        if (scanf_s("%d", &STM) != 1 || STM < STM_MIN || STM > STM_MAX) {
            printf("Invalid input. Please enter a number between %d and %d.\n", STM_MIN, STM_MAX);
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    while (1) {
        printf("percentile probability 'p'(%d~%d) >> ", PROB_MIN, PROB_MAX);
        if (scanf_s("%d", &PROB) != 1 || PROB < PROB_MIN || PROB > PROB_MAX) {
            printf("Invalid input. Please enter a number between %d and %d.\n", PROB_MIN, PROB_MAX);
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
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


    //���� �ֵ�
    prevCPosition = cPosition;
    prevZPosition = zPosition; // �ʱ�ȭ �� ���� ���� ��ġ ����

}

// �ù� �̵� �Լ�
void moveCitizen() {
    int moveProb = rand() % 100;
    if (moveProb < (100 - PROB)) {
        if (cPosition > 0) {
            cPosition--; // �������� 1ĭ �̵�
            cAggro++; // ��׷� ��ġ ����
            if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX; // ��׷ΰ� AGGRO_MAX�� �ʰ����� �ʵ��� ó��
        }
    }
    else {
        cAggro--; // ��׷� ��ġ ����
        if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN; // ��׷ΰ� AGGRO_MIN���� �۾����� �ʵ��� ó��
    }
    // ��׷� ��ġ�� MIN�� MAX�� ����� �ʵ��� ����
    if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
    if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
}

// ���� �̵� �Լ�
void moveZombie() {
    if (zStunned == 1) {
        return;  // M�� pull�� �������� �� ���� �����̹Ƿ� �������� ����
    }

    if (zTurnCount % 2 != 0) {
        return;  // ¦�� ���� �ƴϸ� �������� ����
    }

    // ���� ���� �߰�
    int moveProb = rand() % 100;
    if (moveProb >= PROB) {
        return;  // Ȯ���� ���� �������� ����
    }

    // �̵��� �� �ִ� ���, ������ �̵� ���� �߰�
    if (cAggro >= mAggro) {
        if (zPosition > 0 && zPosition - 1 != cPosition && zPosition - 1 != mPosition) {
            zPosition--;  // �ù��� �� ���� aggro�� ���� �� �������� �̵�
        }
    }
    else {
        if (zPosition < LEN - 1 && zPosition + 1 != cPosition && zPosition + 1 != mPosition) {
            zPosition++;  // M�� �� ���� aggro�� ������ ���������� �̵�
        }
    }
}

// M �̵� �Լ�
void moveM() {

    // ����� �پ����� ���
    if (abs(mPosition - zPosition) == 1) {
        // �̵����� �ʵ��� ������ 0���� ����
        printf("M's movement (0: stay) >> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || mAction != MOVE_STAY) {
                printf("Invalid input. Please enter 0.\n");
                clearInputBuffer();
            }
            else {
                mAggro--;
                clearInputBuffer();
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
                clearInputBuffer();
            }
            else {
                mAggro--; // ��׷� ��ġ ����
                clearInputBuffer();
                break;
            }
        }

        // ���ÿ� ���� ��ġ ����
        if (mAction == MOVE_LEFT && mPosition > 0) {
            mPosition--;
        }

    }

    // M�� �̵� �� aggro ��ġ�� MIN ���� ����� �ʵ��� ����
    if (mPosition > 0 && mAction == MOVE_LEFT) {
        mAggro++; // �̵��ϸ� aggro ��ġ ����
        if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX; // AGGRO_MAX�� �ʰ����� �ʵ��� ����
    }
    else {
        mAggro--; // �̵����� �ʰų� ���� �ε����� aggro ��ġ ����
        if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN; // AGGRO_MIN�� ����� �ʵ��� ����
    }
    // ��׷� ��ġ�� MIN�� MAX�� ����� �ʵ��� ����
    if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;
    if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;

    // M�� ���׹̳��� STM_MAX�� �ʰ����� �ʵ��� ����
    if (STM > STM_MAX) STM = STM_MAX;
    // M�� ���׹̳��� STM_MIN �̸����� �������� �ʵ��� ����
    if (STM < STM_MIN) STM = STM_MIN;
}

// �ù� �ൿ ó�� �Լ�
void performCAction() {
    // �ù��� �ൿ�� �ƹ��͵� ���� ����
}

// ���� �ൿ ó�� �Լ�
void performZAction() {
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            zAction = 1;  // ���� �ù��� ����
        }
        else {
            STM--;
            zAction = 2;  // ���� M�� ����
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        zAction = 1;  // ���� �ù��� ����
    }
    else if (abs(zPosition - mPosition) == 1) {
        STM--;
        zAction = 2;  // ���� M�� ����
    }
    else {
        zAction = 0;  // ���� �ƹ��͵� ���� ����
    }
}


// M �ൿ ó�� �Լ�
void performMAction() {
    // �پ��ִ� ���
    if (abs(mPosition - zPosition) == 1) {
        printf("M's action (0: rest, 1: provoke, 2: pull) >> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice < 0 || choice > 2)) {
                printf("Invalid input. Please enter a number between 0 and 2.\n");
                clearInputBuffer();
            }
            else {
                mAction = choice;
                clearInputBuffer();
                break;
            }
        }
    }
    else {
        // �پ����� ���� ��쿡�� �̵��̳� provoke ������ �� �ֵ���
        printf("M's action (0: rest, 1: provoke) >> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice != ACTION_REST && choice != ACTION_PROVOKE)) {
                printf("Invalid input. Please enter 0 or 1.\n");
                clearInputBuffer();
            }
            else {
                mAction = choice;
                clearInputBuffer();
                break;
            }
        }
    }

    // ���ÿ� ���� aggro ��ġ ����
    if (mAction == ACTION_REST) {
        STM++; // �޽� �� stamina ����
        mAggro--; // �޽� �� aggro ��ġ ����
        printf("M rests (aggro: %d, stamina: %d)\n", mAggro, STM);
        if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN; // AGGRO_MIN�� ����� �ʵ��� ����
    }
    else if (mAction == ACTION_PROVOKE) {
        mAggro = AGGRO_MAX; // provoke �� aggro ��ġ �ִ밪���� ����
        printf("M provokes (aggro: %d, stamina: %d)\n", mAggro, STM);
        
    }
    else if (mAction == ACTION_PULL) {
        int pullSuccess = rand() % (100 - PROB); // PULL ���� Ȯ�� ���
        if (pullSuccess) {
            zStunned = 1; // ���� �������� ǥ��
            printf("M pulled zombie... Next turn, it can't move\n");
        }
        else {
            zStunned = 0;
            printf("M failed to pull zombie\n");
            
        }
        mAggro += 2; // PULL �� ��׷� 2 ����
        STM--; // PULL �� ���¹̳� 1 ����
        if (STM < STM_MIN) STM = STM_MIN; // STM_MIN ���Ϸ� �������� �ʵ��� ����
    }
    // M�� ���׹̳��� STM_MAX�� �ʰ����� �ʵ��� ����
    if (STM > STM_MAX) STM = STM_MAX;
    // M�� ���׹̳��� STM_MIN �̸����� �������� �ʵ��� ����
    if (STM < STM_MIN) STM = STM_MIN;
    // ��׷� ��ġ�� MIN�� MAX�� ����� �ʵ��� ����
    if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;
    if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;
}




// �ù� �ൿ ��� ��� �Լ�
void printCActionResult() {
    if (cPosition == 2) {
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printf("YOU WIN!\n");

        exit(0); // ���� ����
    }
    else {
        printf("Citizen does nothing\n");
    }
}


// ���� �ൿ ��� ��� �Լ�
void printZActionResult() {
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            exit(0); // ���� ����
        }
        
        else {
            printf("Zombie attacks M! (stamina: %d)\n", STM);
            if (STM == 1) {
                printf("GAME OEVER! m dead...");
                exit(0);
            }
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        printf("Zombie attacks citizen!\nGAME OVER!\n");
        exit(0); // ���� ����
    }
    else if (abs(zPosition - mPosition) == 1) {
        printf("Zombie attacks M! (stamina: %d)\n", STM);
        if (STM == 1) {
            printf("GAME OEVER! m dead...");
            exit(0);
    }
    else {
        printf("Zombie does nothing\n");
    }

   

    }
}



// ���� ��ġ�� ���� ��ġ�� ���Ͽ� �޶��������� Ȯ���ϴ� �Լ�
int hasMoved(int prevPosition, int currentPosition) {
    return prevPosition != currentPosition;
}

// �ù� �̵� ��� ��� �Լ�
void printCitizenMoveResult(int prevPosition) {
    if (prevPosition != cPosition) {
        printf("Citizen: %d -> %d (aggro: %d)\n", prevPosition, cPosition, cAggro);
    }
    else {
        printf("Citizen: stay %d (aggro: %d)\n", cPosition, cAggro);
    }
}



// ���� �̵� ��� ��� �Լ�
void printZombieMoveResult(int prevPosition) {
    if (zStunned == 1 && mAction == ACTION_PULL) {
        printf("Zombie: stay %d (cannot move PULL)\n", zPosition);
    }
    else if (zTurnCount % 2 != 0) {
        printf("Zombie: stay %d (cannot move)\n", zPosition);
    }
    else {
        if (prevPosition != zPosition) {
            printf("Zombie: %d -> %d\n", prevPosition, zPosition);
        }
        else {
            printf("Zombie: stay %d\n", zPosition);
        }
    }

    //���� �ֵ�
    prevCPosition = cPosition;
    prevZPosition = zPosition; // �ʱ�ȭ �� ���� ���� ��ġ ����

    zStunned = 0;
}



// M �̵� ��� ��� �Լ�
void printMMoveResult() {
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

        //�ʱ� ȭ�� <�̵�>������ 
        moveCitizen();
        moveZombie();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);

        printf("\n");

        //<�̵�>������ ���
        moveM();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printMMoveResult();
        printf("\n");

        //<�ൿ>������ 
        performCAction();
        performZAction();
        //<�ൿ>������ ���
        printCActionResult();
        printZActionResult();
        printf("\n");
        //<�ൿ>������ ���
        performMAction();
        
        printf("\n");

        // zTurnCount ����
        zTurnCount++;
        
    }

    return 0;
}
