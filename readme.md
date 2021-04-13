Command line argument로 유저에게 인풋 받기

프로그램 실행 방법: ./main $1 $2

Input file format:
줄마다 row col 형식으로 fill-in할 cell을 명시

예시: \
1 2 \
2 3

알고리즘:
board 자체에서 매번 neighbour count연산을 하면 efficiency가 좋지 않기 때문에, neighbour count를 담당하는 vector를 만들었습니다.
살아있는 Cell을 만났을때 주변 cell에게 이웃이 하나 늘어났다고 +1 업데이트 해줍니다.
N 과 M이 클수록 dead cell이면서 이웃이 없는 cell의 개수가 많기 때문에 neighbour count를 미리 알고 있으면 이런 Cell을 skip 할 수 있어서 연산이 빨라집니다.
