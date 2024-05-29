/* 20192816 김도엽
   부산헹(2) 
    2-1 함수 정리 
    2-2 수정 / 입력 다시받기, 어그로, 체력, <이동><행동>페이즈 모두 완료
    2-3 이동 / 조건및 예외처리 모두 완료
    2-4 행동 / <행동>페이즈 순서, 좀비 행동 규칙, 체력감소, 게임오버, 탈출시 아웃트로 모두완료
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define LEN_MIN 15 
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10 
#define PROB_MAX 90
#define AGGRO_MIN 1 
#define AGGRO_MAX 5 
#define MOVE_LEFT 1 
#define MOVE_STAY 0 
#define ACTION_REST 0  
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int LEN, PROB, STM;
int cPosition, zPosition, mPosition;
int cAggro = 1, mAggro = 1;
int zTurnCount = 0;
int zStunned = 0;
int mAction ; 
int zAction ;
int prevCPosition;
int prevZPosition; 



void ending() {
    printf("\n\n\n");
    printf(" ~:-     ::.   .~:::~.    ,::     -:-       ~::    .::-    ~:. -::   ::~     ~:,         ~:,    \n");
    printf(" $@#,   ~@@   -$@@@@@$,   :@@     =@*       ;@@    ~@@=    @@: *@@   @@@.    #@!         #@;    \n");
    printf("  @@~   @@:  -@@@=*#@@@   :@@     =@*        @@.   ;@@$.  :@#- *@@   @@@$    #@!         #@;    \n");
    printf("  *@@  ;@#. .#@$.   -@@=  :@@     =@*        #@=   #@@@~  *@=  *@@   @@@@~   #@!         #@;    \n");
    printf("  ,@@; $@!  :@@~     ;@@. :@@     =@*        ;@@   @$@@~  *@=  *@@   @@;@#.  #@!         #@;    \n");
    printf("   !@#;@#.  ;@#      :@@: :@@     =@*        ~@@  ;@:*@! .$@:  *@@   @@.#@*  #@!         #@;    \n");
    printf("   -#@$@!   !@#      ~@@! :@@     =@*        -@@: @@~.@@ -@@   *@@   @@.,#@~ #@!         #@;    \n");
    printf("    $@@@    *@#      ~@@; :@@     =@*         =@* @#, @@ -@@   *@@   @@. !@$.#@!         #@;    \n");
    printf("    .@@~    ;@#      :@@~ ~@@     =@*         *@*;@*  *@:;@:   *@@   @@.  $@;$@!         #@:    \n");
    printf("     @@,    :@@:     *@@  .@@    ,#@!         :@$$@*  ~@##@    *@@   @@.  -@@#@!         ~:,    \n");
    printf("     @@,     $@@,   ;@@:   @@$   :@@,          @@@@-  -@@@@    *@@   @@.   ~@@@!                \n");
    printf("     @@,     ~#@@$=@@@=    !@@#=#@@=           @@@@   .$@@!    *@@   @@.   .$@@!        .#@;    \n");
    printf("     @@,      -*@@@@@:.    .!@@@@@;            ;@@$    *@@~    *@@   @@.    -@@!        .#@;    \n");
    printf("\n\n\n");

}


//2-1 각 함수들은 1개의 함수가 1가지의 작업만 하도록 설정 
void printPattern(int length) {
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n");
}

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

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void MAXMIN() {
    if (STM > STM_MAX) STM = STM_MAX;
    if (STM < STM_MIN) STM = STM_MIN;
    if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;
    if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;
    if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
    if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
}

// 2-2 입력값 처리 유효값이 입력될 때까지 다시 입력받기 , 모든 입력에 적용 
void initializeGame() {
    MAXMIN();
    srand((unsigned int)time(NULL));

    while (1) {
        printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
        if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {
          
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    while (1) {
        printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
        if (scanf_s("%d", &STM) != 1 || STM < STM_MIN || STM > STM_MAX) {
    
            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    while (1) {
        printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
        if (scanf_s("%d", &PROB) != 1 || PROB < PROB_MIN || PROB > PROB_MAX) {
           
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

    cAggro = 1; 
    mAggro = 1; 

    printf("\n\n");
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n");


    prevCPosition = cPosition;
    prevZPosition = zPosition; 

}



// 2-3 이동페이즈 시민, 좀비, 마동석의 이동과 예외처리
// 시민 이동 함수
void moveCitizen() {
    int moveProb = rand() % 100;
    if (moveProb < (100 - PROB)) {
        if (cPosition > 0) {
            cPosition--;     
        }
    }

}

// 좀비 이동 함수
void moveZombie() {
    if (zStunned == 1) {
        return;  
    }

    if (zTurnCount % 2 != 0) {
        return; 
    }

    int moveProb = rand() % 100;
    if (moveProb >= PROB) {
        return;  
    }

    if (cAggro >= mAggro) {
        if (zPosition > 0 && zPosition - 1 != cPosition && zPosition - 1 != mPosition) {
            zPosition--; 
        }
    }
    else {
        if (zPosition < LEN - 1 && zPosition + 1 != cPosition && zPosition + 1 != mPosition) {
            zPosition++;  
        }
    }
}

//동석 이동
void moveM() {

    if (abs(mPosition - zPosition) == 1) {
  
        printf("madongseok move (0: stay)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || mAction != MOVE_STAY) {
                printf("madongseok move (0: stay)>> ");
                clearInputBuffer();
            }
            else {
              
                clearInputBuffer();
                break;
            }
        }
    }
    else {
   
        printf("madongseok move (0: stay, 1: move left)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || (mAction != MOVE_STAY && mAction != MOVE_LEFT)) {
                printf("madongseok move (0: stay, 1: move left)>> ");
                clearInputBuffer();
            }
            else {
            
                clearInputBuffer();
                break;
            }
        }
        if (mAction == MOVE_LEFT && mPosition > 0) {
            mPosition--;
        }
    }
}


//2-4 행동 페이즈 시민, 좀비, 마동석 행동과 행동 예외처리


void performZAction() {
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            zAction = 1; 
        }
        else {
            
            zAction = 2; 
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        zAction = 1;  
    }
    else if (abs(zPosition - mPosition) == 1) {
     ;
        zAction = 2; 
    }
    else {
        zAction = 0;  
    }
}


void performMAction() {
    int STMa = STM;
    int mAggrom = mAggro;

  
    if (abs(mPosition - zPosition) == 1) {
        printf("madongseok action(0: rest, 1: provoke, 2: pull)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice < 0 || choice > 2)) {
                printf("madongseok action(0: rest, 1: provoke, 2: pull)>> ");
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
     
        printf("madongseok action(0: rest, 1: provoke)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice != ACTION_REST && choice != ACTION_PROVOKE)) {
                printf("madongseok action(0: rest, 1: provoke)>> ");
                clearInputBuffer();
            }
            else {
                mAction = choice;
                clearInputBuffer();
                break;
            }
        }
    }
    printf("\n");
  
    if (mAction == ACTION_REST) {
        printf("\nmadongseok rests...\n");
        STM++;
        mAggro--; 
        MAXMIN();
        printf("madongseok : %d (aggro: %d -> %d, stamina: %d -> %d)\n",mPosition, mAggrom, mAggro, STMa, STM);
        
    }
    else if (mAction == ACTION_PROVOKE) {
        printf("\nmadongseok provoked zombie...\n");
        MAXMIN();
        int mAggrom = mAggro;
        printf("madongseok : %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom, mAggro = AGGRO_MAX , STM);
        mAggro = AGGRO_MAX;
    }
    else if (mAction == ACTION_PULL) {
        int mAggrom = mAggro;
  
        mAggro += 2; 
        STM--;
        MAXMIN();
        int pullSuccess = rand() % (100 - PROB); 
        if (pullSuccess) {
            zStunned = 1;
            printf("madongseok pulled zombie... Next turn, it can't move\n");
            printf("madongseok : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }
        else {
            zStunned = 0;
            printf("madongseok failed to pull zombie\n");
            printf("madongseok : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }

    }
    printf("\n");
}

void performCAction() {
    //아무런 행동 X
}
void printCActionResult() {
    if (cPosition == 1) {
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
            printf("YOU WIN! ···\n");
            Sleep(2000); 
            system("cls");
            ending();
            exit(0);
    }
    else {
        printf("Citizen does nothing.\n");
    }
}

void printZActionResult() {
    int STMa = STM;
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            printf("Zombie attacks citizen! (aggro: $d vs. $d)\nGAME OVER!\n");
            exit(0); 
        }
        else {
            STM--;
            MAXMIN();
            printf("Zombie attacks madongseok! (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n",cAggro, mAggro, STMa, STM);
            if (STM == 1) {
                printf("GAME OEVER! madongseok dead...");
                exit(0);
            }
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        printf("Zombie attacks citizen!\nGAME OVER!\n");
        exit(0); 
    }
    else if (abs(zPosition - mPosition) == 1) {
        STM--;
        MAXMIN();
        printf("Zombie attacks madongseok! (stamina: %d -> %d)\n", STMa, STM);
        if (STM == 1) {
            printf("GAME OEVER! madongseok dead...");
            exit(0);
        }
    }
    if ((zAction) == 0) {
        printf("zombie attcked nobody.\n");
    }
}

int hasMoved(int prevPosition, int currentPosition) {
    return prevPosition != currentPosition;
}


void printCitizenMoveResult(int prevPosition) {
    int cAggroc = cAggro;
    if (prevPosition != cPosition) {
        cAggro++;
        MAXMIN();
        printf("Citizen: %d -> %d (aggro: %d -> %d)\n", prevPosition, cPosition, cAggroc, cAggro);
    }
    else {
        
        cAggro--;
        MAXMIN();
        printf("Citizen: stay %d (aggro: %d -> %d)\n", cPosition, cAggroc, cAggro);
    }
}




void printZombieMoveResult(int prevPosition) {
    MAXMIN();
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


    prevCPosition = cPosition;
    prevZPosition = zPosition; 

    zStunned = 0;
}



void printMMoveResult() {
   
    int mAggrom = mAggro;

    if (mAction == MOVE_LEFT && mPosition > 0) {
        mAggro++;
        MAXMIN();
        printf("madongseok: %d -> %d (aggro: %d -> %d, stamina: %d)\n", mPosition +1, mPosition, mAggrom, mAggro, STM);
        
    }
    else {
        mAggro--;
        MAXMIN();
        printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom,mAggro, STM);

    }
}


//메인
int main(void) {
    
    printf("잠시후 기차가 출발합니다...\n\n");
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
    
    initializeGame();
    srand((unsigned int)time(NULL));

    while (1) {
     
        moveCitizen(); 
        moveZombie(); 
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition); 
        printCitizenMoveResult(prevCPosition); 
        printZombieMoveResult(prevZPosition); 
        printf("\n");

        moveM();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printMMoveResult();
        printf("\n");


        performCAction();
        performZAction();
    
        printCActionResult();
        printZActionResult();

      
        performMAction();
     
        printf("\n");

     
        zTurnCount++;
        
    }

    return 0;
}
