//人臉辨識+座標+傳到手機+手機傳名字給電腦供電腦判斷
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
using namespace cv;
#include <stdio.h>
#include <time.h>
#include <conio.h>
using namespace cv;
using namespace std;

string box_text[100];//放名字
string box_tex[10];
string box_text_2[50];//放臉中心座標
//string box_text_3;//放2眼中心座標
char message[100];
//string text="no appears";
//string directory;
string path;
int t;
SOCKADDR_IN clinetAddr;
SOCKADDR_IN addr;
int addrlen = sizeof(addr);
int construct=0;
int construct1=0;
int construct2=0;
int construct3=0;
int construct4=0;
int construct5=0;
unsigned int facesize;
int retval,continueflag=1;
int flag=0;
int mno=0;
int jno=0;
int bno=0;
int eno=0;
int check=0;
int checks;
int endtimer;
int noface=0;
fd_set rfds;//宣告一個描述文件字fd的集合叫做rfds
struct timeval tv;

//time_t start, end;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') 
{
    ifstream file("D:/chiren/tesr/database.txt", ifstream::in);
    if (!file)
	{
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line))
	{
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty())
		{
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}
/*
DWORD WINAPI checkface(LPVOID lp){
	int checkflag=(int)lp;
	if(facesize>0)
	{
		checkflag=1;
		check = checkflag;
	}
	else
	{ 
		check = checkflag;
	}
	return check;
}

DWORD WINAPI timer(LPVOID lp)
{
	clock_t start, end;
	//DWORD kthread;
	endtimer=1;
	start = clock();
	check=0;
	end = clock();
	while(((end-start)/1000)<5){
	//CreateThread(NULL, 0,checkface,(LPVOID)check,0, &kthread);


		end = clock();
		if(facesize>0)
	{
		//checkflag=1;
		check = 1;
	}
 
	}
	//check=0;
	endtimer=0;
	return 1;
	/*int facesize=(int)lp;
	fd_set rfds;
    struct timeval tv;
	while(1)
	{
	FD_ZERO(&rfds); 
    FD_SET(facesize, &rfds); 
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	retval = select(facesize+1, &rfds, NULL, NULL, &tv);
	cout<<retval;
	if(retval == -1) {
			//perror("select()");
			//continueflag=0;
		continueflag=1;
		}
	if(retval>=1)
	{
		continueflag=1;
	}
	else if(retval==0)
	{
		continueflag=0;
	}
	}
	return 1;
	//exit(0);
}*/

DWORD WINAPI time(LPVOID lp)
{
	
	clock_t start, end;
	int checkflag = 0;
	while (true){
		if(facesize>0)
	{
		checkflag=1;
		check = checkflag;
	}
		else if(facesize==0)
	{
		//cout<<"no face";
		checkflag = 0;
	start = clock();
	end = clock();
	
	//CreateThread(NULL, 0,timer,(LPVOID)facesize,0, &tthread);
	while(((end-start)/1000)<3 && checkflag!=1){
	//CreateThread(NULL, 0,checkface,(LPVOID)check,0, &kthread);


		end = clock();
		if(facesize>0)
	{
		checkflag=1;
		
	}
 
	}
	}
		//cout<<checkflag;
	if (checkflag == 1)
	{
		check=1;
	}
	else if (checkflag == 0)
	{
		check=0;
	}
	cout<<check;
	}
	
	return 0;
}

DWORD WINAPI receive_cmds_3(LPVOID lp)
{
	
	flag++;

	int dir=(int)lp;
	string directory;
	int no;
	string number;
	stringstream ss;
	ss<<number;
	//ss>>no;
	//cout<<flag;
	DWORD rthread;
	check=1;
	//while (endtimer==0){
	
	//}
	if(dir==0)
	{
		directory=="GG";
	}
	if(dir==1){
		directory="Bradley1";
		no =bno;
	}
	if(dir==2){
		directory="Ellen1";
		no=eno;
	}
	if(dir==3){
		directory="Jennifer1";
		no=jno;
	}
	else if(dir==4){
		directory="Mary1";
		no=mno;
	}
	
	

	ss>>no;

	//path="C:/xampp/htdocs/dashboard/"+directory+ss.str()+".avi";
	path="C:/xampp/htdocs/dashboard/"+directory+".avi";
	//DWORD sthread;
	//DWORD tthread;
	VideoCapture cap(0);
	Mat frame;
	VideoWriter writed;
	Size videoSize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	writed.open(path, CV_FOURCC('D', 'I', 'V', 'X'), 8, videoSize);
	CreateThread(NULL, 0,time,(LPVOID)dir,0, &rthread);
	//CreateThread(NULL, 0,timer,(LPVOID)facesize,0, &tthread);
	while(check!=0)
	//while (kbhit()==0) //while(1)
	{
	cap >> frame;
	writed.write(frame);
	/*if (facesize<0)
	{
	break;
	}*/
	//system("D:/workspace/upload/src/main/resources/upload.jar");
	}



	if (directory=="Bradley1"){
		system("D:/temp/ffmpegsample/Debug/ffmpegsample.exe");
	}
	else if (directory=="Ellen1"){
		system("D:/etemp/ffmpegsample/Debug/ffmpegsample.exe");
	}
	if (directory=="Jennifer1"){
		system("D:/jtemp/ffmpegsample/Debug/ffmpegsample.exe");
	}
	else if (directory=="Mary1"){
		system("D:/mtemp/ffmpegsample/Debug/ffmpegsample.exe");
	}
	//system("D:/workspace/upload/src/main/resources/upload.jar");
	construct=0;
	return 0;
	//exit(1);
	//exit(0);
}
/*DWORD WINAPI send_filename(LPVOID lp)
{
	SOCKET sConnect; 
	int cat=(int)lp;

}*/

DWORD WINAPI Full_Video(LPVOID lp)
{
	VideoCapture cap(0);
	Mat frame;
	Size videoSize = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    VideoWriter writer;
    writer.open("FullVideo.avi", CV_FOURCC('M', 'J', 'P', 'G'), 8, videoSize);
	while(1){
	cap>>frame;
	writer.write(frame);
	//imshow("face_recognize",frame);
				if(cvWaitKey(30)==27)
					break;
	}
	//system("D:/workspace/upload/src/main/resources/upload.jar");
	return 1;
}

string int2str(int &i) {
					string s;
					stringstream ss(s);
					ss << i;

					return ss.str();
				}

int main(int argc, const char *argv[])
{
	//檢查有效的參數  如果沒有分別給下列欄位參數，就印出用法，
	//VideoCapture capture(0);
	
    if (argc != 4)
	{
        cout << "usage: " << argv[0] << " </path/to/haar_cascade> </path/to/csv.ext> </path/to/device id>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        cout << "\t </path/to/csv.ext> -- Path to the CSV file with the face database." << endl;
        cout << "\t <device id> -- The webcam device id to grab frames from." << endl;
        //exit(1);
    }

	//取得分類器、資料庫與視訊的路徑
	//取得分類器、資料庫與視訊的路徑
	string fn_haar = "D:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
	string eye = "D:/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
    string fn_csv = "D:/chiren/tesr/database.txt";
    int deviceId = 0;

	//下面的變數保留圖片與對應的標籤
    vector<Mat> images;
    vector<int> labels;

	try //讀取資料庫
	{
        read_csv(fn_csv, images, labels);
    } 
	catch (cv::Exception& e)//如果輸入csv有問題  印出錯誤訊息
	{
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        exit(1);
    }

	//取得第一張圖的高度  重塑圖片到原來的尺寸
    int im_width = 100;
    int im_height = 100;

	String fisherPath = "D:/chiren/tesr/fisher.xml";//建立訓練檔//建立訓練檔
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();//建立 FaceRecognizer 並訓練

    //model->train(images, labels);//訓練
	//model->save(fisherPath);//儲存訓練檔
	model->load(fisherPath);//讀取訓練檔
	
    //學習辨識 建立人臉檢測 使用指定的haar
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
	CascadeClassifier eye_cascade; 
	eye_cascade.load(eye);

    VideoCapture cap(deviceId);// 取得webcam
    if(!cap.isOpened())//若找不到webcam  顯示錯誤訊息
	{
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    
	//存放目前的frame
    Mat frame;

	int r,receive;
	WSAData wsaData;
	WORD DLLVSERION;
	//DWORD thread;
	DWORD fthread;
	DWORD vthread;
	
	DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL 版本
 
	//用 WSAStartup 開始 Winsocket-DLL  
	//使用Socket之前必須使用WSAStartup函數，此函數在應用程式中用來初始化Windows Sockets DLL
	//只有此函式呼叫成功後，應用程式才可以調用Windows SocketsDLL中的其他API函數
	//否則後面的任何函數都將使用失敗
	//初始化成功則r=0
	r = WSAStartup(DLLVSERION, &wsaData);
	receive= WSAStartup(DLLVSERION, &wsaData);	
	int dir=0;
	//設定位址資訊的資料
	
	CreateThread(NULL, 0,Full_Video,0,0, &fthread);
	
	//等待連線
	
	while(true)
	{
		//if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
		//{
		for(;;) 
		{
			
			cap >> frame;
        
			Mat original = frame.clone();//複製目前的frame
			
			Mat gray;
			cvtColor(original, gray, CV_BGR2GRAY);//將frame轉灰階
			
			vector< Rect_<int> > faces;//在frame尋找臉與眼睛
			vector< Rect_<int> > eye;
			//haar_cascade.detectMultiScale(gray, faces);
			//改變偵測尺寸
			int fsz=50;
			haar_cascade.detectMultiScale( frame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(fsz, fsz) );
			//                                          (     能調整誤判的參數        )
			facesize=faces.size();
			
			
			//if(faces.size()>0)
				//cvDestroyWindow("face_recognizer");
			//將偵測到的臉做辨識並顯示
			//cout<<faces.size();
			for(int i = 0; i < faces.size(); i++) 
			{
				
				
				Rect face_i = faces[i];// Process face by face:
				Mat face = gray(face_i);//將圖片上的臉裁下來
				Mat face_resized;
			
				//改變圖片大小
				resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
           
				//建立辨識的人與相似度的變數
				int predictedLabel = -1;
				double confidence = 0.0;
				model->predict(face_resized, predictedLabel, confidence);

				  int prediction = model->predict(face_resized);
				//String prediction= model->predict(face_resized);
				//if(confidence>=24 && confidence<=30)

				rectangle(original, face_i, CV_RGB(0, 255,0), 1);//畫出檢測臉部的矩形

				if (confidence < 180)
				{
				//列印辨識的人與相似度
				string box_text_3 = format("Prediction = %d, %f", predictedLabel, confidence);

				unsigned int x_axis = faces[i].x;//建立放座標的變數
				unsigned int y_axis = faces[i].y;

				unsigned int aa = faces[i].x;//建立放相似度的變數
				unsigned int bb = faces[i].y;
				//CreateThread(NULL, 0,receive_cmds_3,0,(LPVOID)prediction, &vthread);
				//如果辨識結果為某數
				switch (prediction)
				{

					case 0:
					box_text[i] = format("Bradley");//顯示名字
					//directory="Bradley";
					construct1++;
					continueflag=1;
					dir=1;
					//writer1.open("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 18, videoSize);
					break;
					case 1:
					box_text[i] = format("Ellen");
					construct2++;
					continueflag=1;
					//directory="Ellen";
					dir=2;
					//writer2.open("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 18, videoSize);
				    break;
					case 2:
					box_text[i] = format("Jennifer");
					construct++;
					//directory="Jennifer";
					dir=3;
					//writer3.open("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 18, videoSize);
					//CreateThread(NULL, 0,receive_cmds_3,0,frame, &vthread);
					break;
					case 3:
					box_text[i] = format("Mary");
					//directory="Mary";
					dir=4;
					construct3++;
					//writer4.open("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 18, videoSize);
					break;
					default:
					box_text[i] = format("GG");
					dir=0;
					break;
				}
				if(construct==1||construct1==1||construct2==1||construct3==1)
				{
				CreateThread(NULL, 0,receive_cmds_3,(LPVOID)dir,0, &vthread);
				}
				
				box_text_2[i]=format("(%d,%d)",x_axis,y_axis);//顯示座標
			
				//顯示名字的位置  並確認不把錯誤的值放裡面
				int pos_x = max(face_i.tl().x + 10, 0);
				int pos_y = max(face_i.tl().y + 90, 0);

				//顯示座標的位置  並確認不把錯誤的值放裡面
				x_axis    = max(face_i.tl().x + 10, 0);
				y_axis    = max(face_i.tl().y + 105, 0);

				//顯示相似度的位置
				aa    = max(face_i.tl().x + 10, 0);
				bb    = max(face_i.tl().y + 45, 0);
				//string fsize = int2str(facesize);
				//box_tex[0] = format("(%d)",fsize);
				//box_tex=int2str(facesize);
				//將上面的名字與座標放進圖片
				putText(original, box_text[i], Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
				//putText(original, &box_tex[0], Point(50,50), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0);
				putText(original, box_text_2[i], Point(x_axis,y_axis), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
			//	putText(original, box_text_3, Point(aa,bb), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
				int esz=1;
				eye_cascade.detectMultiScale(face, eye, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(esz,esz) );
				//畫出眼睛並顯示座標
					for (int j = 0; j < eye.size(); j++)
					{
						unsigned int eye_x_axis = eye[j].x;
						unsigned int eye_y_axis = eye[j].y;
						eye_x_axis = max(face_i.tl().x + 10, 0);
						eye_y_axis = max(face_i.tl().y + 120, 0);
						box_text_3 = format("(%d,%d)", eye_x_axis, eye_y_axis);
						putText(original, box_text_3, Point(eye_x_axis, eye_y_axis), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0, 255, 0), 2.0);
					}
						//清空陣列避免爆掉
						t=i;
						box_text[i]='\0';
						box_text_2[i]='\0';
						message[i]='\0';
				}
				//顯示結果
				
		}
		imshow("face_recognizer", original);
				if(cvWaitKey(30)==27)
					break;
		
	}
	}
	//system("D:/workspace/upload/src/main/resources/upload.jar");
    return 0;
}
