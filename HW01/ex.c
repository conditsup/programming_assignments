#include <stdio.h>
#include <Windows.h>

#define MIN_PROBABILITY 10
#define MAX_PROBABILITY 90

int main() {
    int p;
    printf("이동할 확률을 입력하세요 (10에서 90 사이): ");
    if (scanf_s("%d", &p) != 1 || p < MIN_PROBABILITY || p > MAX_PROBABILITY) {
        printf("잘못된 입력입니다. 프로그램을 종료합니다.\n");
        return 1;
    }

    // 입력된 확률에서 100을 빼어 Z가 이동할 비율 계산
    double move_percent = 100.0 - p;

    // Z의 현재 위치를 나타내는 변수
    int z_position = 0;

    // 프로그램을 종료하기 위한 조건 설정
    int end_position = 100; // 이동할 총 거리

    while (z_position < end_position) {
        // 현재 위치 출력
        printf("Z의 현재 위치: %d\n", z_position);

        // 1초 대기
        Sleep(1000); // 밀리초 단위이므로 1000 밀리초 = 1초

        // 이동할 비율에 따라 Z 이동
        z_position += move_percent / 100.0;

        // 만약 이동 후 위치가 총 거리를 넘어가면 총 거리로 설정
        if (z_position > end_position) {
            z_position = end_position;
        }
    }

    // Z가 목표지점에 도착했음을 알림
    printf("Z가 목표지점에 도착했습니다.\n");

    return 0;
}
