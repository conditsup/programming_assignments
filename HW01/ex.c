#include <stdio.h>
#include <Windows.h>

#define MIN_PROBABILITY 10
#define MAX_PROBABILITY 90

int main() {
    int p;
    printf("�̵��� Ȯ���� �Է��ϼ��� (10���� 90 ����): ");
    if (scanf_s("%d", &p) != 1 || p < MIN_PROBABILITY || p > MAX_PROBABILITY) {
        printf("�߸��� �Է��Դϴ�. ���α׷��� �����մϴ�.\n");
        return 1;
    }

    // �Էµ� Ȯ������ 100�� ���� Z�� �̵��� ���� ���
    double move_percent = 100.0 - p;

    // Z�� ���� ��ġ�� ��Ÿ���� ����
    int z_position = 0;

    // ���α׷��� �����ϱ� ���� ���� ����
    int end_position = 100; // �̵��� �� �Ÿ�

    while (z_position < end_position) {
        // ���� ��ġ ���
        printf("Z�� ���� ��ġ: %d\n", z_position);

        // 1�� ���
        Sleep(1000); // �и��� �����̹Ƿ� 1000 �и��� = 1��

        // �̵��� ������ ���� Z �̵�
        z_position += move_percent / 100.0;

        // ���� �̵� �� ��ġ�� �� �Ÿ��� �Ѿ�� �� �Ÿ��� ����
        if (z_position > end_position) {
            z_position = end_position;
        }
    }

    // Z�� ��ǥ������ ���������� �˸�
    printf("Z�� ��ǥ������ �����߽��ϴ�.\n");

    return 0;
}
