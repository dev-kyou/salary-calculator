#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include<string> //必要なファイルをinclude.
#define N 10

void delay(int millseconds) {
	clock_t start_time = clock();
	while (clock() < start_time + millseconds);
}
//time.hを利用し指定秒数分処理を遅らせる関数.

int main(void) {
	int pay,work_time, day_of_work, toll;
	int month_pay, month_toll, ALL; //必要な引数の宣言.
	char str[10];

	time_t nowTime = time(NULL);
	struct tm local_time;
	char time_str[256];
	localtime_s(&local_time,&nowTime);
	strftime(time_str, 256, "%Y/%m", &local_time);
	printf("%s\n\n", time_str); //time関数を用いて起動時の年月を取得.

	FILE* file;
	errno_t error;

	error = fopen_s(&file, "pay_log.data", "a");
	fclose(file);
	fopen_s(&file, "pay_log.data", "r");
	//pay_log.dataというログファイルの作成.

	if (fgets(str,N,file) == NULL) { //pay_log.dataを参照し記載があるかどうかの判定.
		fclose(file);
		fopen_s(&file, "pay_log.data", "w");
		printf("時給を入力してください。\n");
		scanf_s("%d", &pay);
		if (pay< 0) { //時給が負である場合には不自然な値であると判断してアプリを終了する.
			printf("\x1b[31m不自然な値が入力されました。入力を確認してください。\n\x1b[0m");
			delay(1000);
			return 0;
		}
		fprintf(file, "%d", pay);
		//pay_log.dataをwモードで開きscanf関数でユーザに時給の入力を行ってもらいpay_log.dataに書き込む.

		
	}
	fclose(file);
	
	fopen_s(&file, "pay_log.data", "r");
	char num[20];
	while (fgets(num,N, file) != NULL) //pay_log.dataの中の文字をNULLになるまで読み取る.

	pay=atoi(num); //文字列を整数型に変換.

		printf("１か月の労働時間を入力してください。\n\x1b[33m時給が変わった場合は-999を入力してください。(その後の入力はすべて0にしてください。)\n\x1b[0m");
		scanf_s("%d", &work_time);
		printf("1か月の出勤日数を入力してください。\n");
		scanf_s("%d", &day_of_work);
		printf("1回の出勤における交通費を入力してください。\n");
		scanf_s("%d", &toll);
		//労働時間,出勤日数,交通費をそれぞれユーザに入力してもらう.
		
		if (work_time > 744 or (work_time < 0 and work_time !=-999) or day_of_work > 31 or day_of_work < 0) {
			printf("\x1b[31m不自然な値が入力されました。入力を確認してください。\n\n\x1b[0m");
			delay(1000);
			return 0;
		}
		//

	fclose(file);
	if (work_time == -999) {
		remove("pay_log.data");
		return 0;
	}
	//work_timeに-999が入力されるとpay_log.dataを削除しリセットする.
	

	month_pay = pay* work_time;
	
	if (month_pay >= 105000) {
		printf("\n今月の給与所得が105,000円を超えたので源泉徴収が行われる可能性があります。\n\n");
	} //月間収入によっては源泉徴収があるのでその旨を表示.令和8年度版を参照.
	month_toll = day_of_work * toll;
	
	ALL = month_pay + month_toll;
	
	//月間労働分の給与計算と交通費を加算した値を変数ALLに格納.

	printf("\n1か月の給料は%d円です。\n交通費が%d円です。\n合計で%d円です。\n", (int)month_pay, (int)month_toll, (int)ALL);
	//1か月分の給料と交通費とそれらを加えたALLを表示.


	char diference[64];
	strftime(diference, sizeof(diference), "diference_%Y.data", &local_time);
	//time関数を用いて年を取得しdiference_20YY.dataというファイルを作成.

	FILE* file2;
	errno_t error2;
	error2 = fopen_s(&file2, diference, "a");
	fclose(file2);
	fopen_s(&file2, diference, "r");

	int num2=0,write=0;
	char curr[16];
	while (fgets(curr, 16, file2)) {
		}
	//fiference_20XX.dataの最終行を取得.
	
		num2 = atoi(curr);//取得した文字列を整数型に変換.
	fclose(file2);

	write = num2 + month_pay;
	fopen_s(&file2, diference, "a");
	fprintf_s(file2,"%d\n", write);
	fclose(file2);
	//num2とmonth_payを合算しdiferenceに書き込む.

	int tax = 1500000;
	int dif = tax - write;
	printf("\n150万円の壁における残額は%d円です。\n", dif);
	//所謂150万の壁(2025年度より改正)の残額を表示.
	
	double div = (1 - (static_cast<double>(dif) / tax)) * 100;
	printf("150万円のうち%d%%分稼いでいます。\n", (int)div);
	if (div >=82 ) {
		printf("\x1b[31m所得税の支払いが発生する可能性があります。\n\x1b[0m");
	}
	//150万円以上の収入があった場合に所得税の支払いが発生する可能性がある旨を表示.

	fclose(file2);


	delay(10000);//前述のdelay関数を使用.10秒処理を遅らせている.
	return 0;
}
