//人臉辨識+座標+傳到手機+手機傳名字給電腦供電腦判斷
/*
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
#include <string>
#include <stdio.h>
using namespace cv;
using namespace std;
string box_text[100];//放名字
string box_text_2[50];//放臉中心座標
//string box_text_3;//放2眼中心座標
char message[100];
string text="no appears";
int t;
SOCKADDR_IN clinetAddr;
SOCKADDR_IN addr;
int addrlen = sizeof(addr);
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
DWORD WINAPI receive_cmds_2(LPVOID lpParam)
{
	//接收手機端的訊息
			 SOCKET sConnect = (SOCKET)lpParam;
				while(recv(sConnect, message, sizeof(message),0))
				{
					//cout<<box_text[t].c_str()<<endl;
				
				if (strncmp(box_text[t].c_str() ,message,100)==0)
				{ 
					cout<<box_text[t].c_str()<<endl;
					send(sConnect, box_text_2[t].c_str(), (int)strlen(box_text_2[t].c_str()), 0);
				}
				}
				return 1;
}
DWORD WINAPI receive_cmds(LPVOID lpParam) 
{
 // cout<<"thread create"<<endl;
    //cout << box_text[t] << endl;
	//cout << message << endl;
  // set our socket to the socket passed in as a parameter   
  SOCKET sListen = (SOCKET)lpParam;
  SOCKET sConnect; 
  
  // buffer to hold our recived data
  char buf[100];
  // buffer to hold our sent data
  char sendData[100];
  // for error checking 
  int res;
  DWORD thread2; 
  while(true)
  {

	  if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
	  {
		  Sleep(10);
		  cout<<"sm";
		  CreateThread(NULL, 0,receive_cmds_2,(LPVOID)sConnect, 0, &thread2);
	  }
  }

  return 1;
}
int main(int argc, const char *argv[])
{
	//檢查有效的參數  如果沒有分別給下列欄位參數，就印出用法，
	VideoCapture capture(0);
	Size videoSize = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
    VideoWriter writer;
    writer.open("VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, videoSize);
    if (argc != 4)
	{
        cout << "usage: " << argv[0] << " </path/to/haar_cascade> </path/to/csv.ext> </path/to/device id>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        cout << "\t </path/to/csv.ext> -- Path to the CSV file with the face database." << endl;
        cout << "\t <device id> -- The webcam device id to grab frames from." << endl;
        //exit(1);
    }

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

	String fisherPath = "D:/chiren/tesr/fisher.xml";//建立訓練檔
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
	DWORD thread; 
	DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL 版本
 
	//用 WSAStartup 開始 Winsocket-DLL  
	//使用Socket之前必須使用WSAStartup函數，此函數在應用程式中用來初始化Windows Sockets DLL
	//只有此函式呼叫成功後，應用程式才可以調用Windows SocketsDLL中的其他API函數
	//否則後面的任何函數都將使用失敗
	//初始化成功則r=0
	r = WSAStartup(DLLVSERION, &wsaData);
	receive= WSAStartup(DLLVSERION, &wsaData);
	
	//宣告 socket 位址資訊(不同的通訊,有不同的位址資訊,所以會有不同的資料結構存放這些位址資訊)
	
 
	//建立 socket
	SOCKET sListen; //listening for an incoming connection
	SOCKET sConnect; //operating if a connection was found

	//設定位址資訊的資料
	addr.sin_addr.s_addr = inet_addr("140.134.26.134");//10.22.25.21  173.1
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1999);
 
	//設定 Listen
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);//SOMAXCONN: listening without any limit	
	
	//等待連線
	
	CreateThread(NULL, 0,receive_cmds,(LPVOID)sListen, 0, &thread);
	//if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
	while(true)
	{
		//if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
		{
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
			if(faces.size>0)
			{
				writer.write(frame);
			}
			//將偵測到的臉做辨識並顯示
			for(int i = 0; i < faces.size(); i++) 
			{
				//清空陣列避免爆掉
				t=i;
				box_text[i]='\0';
				box_text_2[i]='\0';
				message[i]='\0';

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

				//如果辨識結果為某數
				switch (prediction)
				{
					case 0:
					box_text[i] = format("Bradley");//顯示名字
					break;
					case 1:
					box_text[i] = format("Ellen");
				    break;
					case 2:
					box_text[i] = format("Jennifer");
					break;
					case 3:
					box_text[i] = format("Mary");
					break;
					default:
					box_text[i] = format("GG");
					break;
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

				//將上面的名字與座標放進圖片
				putText(original, box_text[i], Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
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
				}
				//顯示結果
				imshow("face_recognizer", original);

				//接收手機端的訊息
				char key = (char) waitKey(20);
				printf("%c",key);

				//退出迴圈
				if(key == 27)//ASCII 27 對應到esc
					break;
				//}
				//程式等待20ms
				
			}
		}
	}
	}
    return 0;
}*/