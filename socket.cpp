#include <winsock2.h> // 소켓프로그래밍을 하기위해 필수불가결한 소켓해더
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <locale.h> // 한글출력이 제대로안되 찾아본 해더 아래의 
					  // _wsetlocale( LC_ALL, L"korean" );를 사용하여 출력이 가능하도록 함
#define BUFSIZE 512
#pragma comment(lib,"ws2_32.lib") //pragma 가 새로운 함수를 만들때 사용한다고 들었는데
								  //여기서는 "ws2_32.lib"라는 라이브러리를 사용한듯하다
int main(){
//	_wsetlocale( LC_ALL, L"korean" );
	int retval;
	char buf[BUFSIZE];
	FILE *fp = fopen("test.html", "w");//출력내용을 파일에 저장하기위해 파일을연다
	WSADATA wsa; // WSADATA 구조체는 윈도우 소켓 초기화와 호출에의한 정보를 저장하는데 사용되는 함수
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) // 소켓 버전일 비교해주고 버전이 맞을경우 0을반환하는데
		return -1;							 // 0을반환하지 못하고 오류가발생할경우 -1값을 반환
	//socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // 소켓의 연결을 확인해주고
	if(sock == INVALID_SOCKET) return 0; // 연결이 맞지않으면 0을리턴
	hostent* remoteHost = gethostbyname("test.gilgil.net");//웹주소를 ip로 바꾸기 위해 사용 
	//connect()
	SOCKADDR_IN serveraddr; //윈도우 소켓이 소켓을 연결할 로컬 또는 원격 끝점 주소를 지정하는 데 사용
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(80);
	serveraddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list));
	/*
	struct sockaddr_in{ 
	short sin_family; 
	unsigned short sin_port; 
	struct in_addr sin_addr; 
	char sin_zero[8]; 
	};
	SOCKADDR_IN 구조체는 위 처럼 정의되어있는데 각 구조에맞추어 입력을 해 준다
	*/
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) return 0;  //소켓에러가난다면 0을 반환
	char msg[500] = "GET / HTTP/1.1\r\n";
	strcat(msg,"Host: test.gilgil.net:80\r\n\r\n");//웹서버로 보낼 헤더 작성 
	send(sock, msg, strlen(msg), 0);//웹서버로 보내기
	while(1){
		if((retval = recv(sock, buf, BUFSIZE, 0)) == 0) break;//서버에서 응답 받기
		printf("%s", buf);//화면에 출력(너무 많아서 짤려서 보임)
		fprintf(fp, "%s", buf);//파일에 저장
		memset(buf, 0, BUFSIZE);//버퍼 지우기
	}
	closesocket(sock);
	fclose(fp); //파일을 닫는다
	WSACleanup();
	return 0;
}