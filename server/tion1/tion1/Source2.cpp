//�H�y����+�y��+�Ǩ���+����ǦW�r���q���ѹq���P�_
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
string box_text[100];//��W�r
string box_text_2[50];//���y���߮y��
//string box_text_3;//��2�����߮y��
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
	//��������ݪ��T��
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
	//�ˬd���Ī��Ѽ�  �p�G�S�����O���U�C���ѼơA�N�L�X�Ϊk�A
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

	//���o�������B��Ʈw�P���T�����|
	string fn_haar = "D:/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
	string eye = "D:/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
    string fn_csv = "D:/chiren/tesr/database.txt";
    int deviceId = 0;

	//�U�����ܼƫO�d�Ϥ��P����������
    vector<Mat> images;
    vector<int> labels;

	try //Ū����Ʈw
	{
        read_csv(fn_csv, images, labels);
    } 
	catch (cv::Exception& e)//�p�G��Jcsv�����D  �L�X���~�T��
	{
        cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
        exit(1);
    }

	//���o�Ĥ@�i�Ϫ�����  ����Ϥ����Ӫ��ؤo
    int im_width = 100;
    int im_height = 100;

	String fisherPath = "D:/chiren/tesr/fisher.xml";//�إ߰V�m��
    Ptr<FaceRecognizer> model = createFisherFaceRecognizer();//�إ� FaceRecognizer �ðV�m

    //model->train(images, labels);//�V�m
	//model->save(fisherPath);//�x�s�V�m��
	model->load(fisherPath);//Ū���V�m��
	
    //�ǲ߿��� �إߤH�y�˴� �ϥΫ��w��haar
    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);
	CascadeClassifier eye_cascade; 
	eye_cascade.load(eye);

    VideoCapture cap(deviceId);// ���owebcam
    if(!cap.isOpened())//�Y�䤣��webcam  ��ܿ��~�T��
	{
        cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
        return -1;
    }
    
	//�s��ثe��frame
    Mat frame;

	int r,receive;
	WSAData wsaData;
	WORD DLLVSERION;
	DWORD thread; 
	DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL ����
 
	//�� WSAStartup �}�l Winsocket-DLL  
	//�ϥ�Socket���e�����ϥ�WSAStartup��ơA����Ʀb���ε{�����ΨӪ�l��Windows Sockets DLL
	//�u�����禡�I�s���\��A���ε{���~�i�H�ե�Windows SocketsDLL������LAPI���
	//�_�h�᭱�������Ƴ��N�ϥΥ���
	//��l�Ʀ��\�hr=0
	r = WSAStartup(DLLVSERION, &wsaData);
	receive= WSAStartup(DLLVSERION, &wsaData);
	
	//�ŧi socket ��}��T(���P���q�T,�����P����}��T,�ҥH�|�����P����Ƶ��c�s��o�Ǧ�}��T)
	
 
	//�إ� socket
	SOCKET sListen; //listening for an incoming connection
	SOCKET sConnect; //operating if a connection was found

	//�]�w��}��T�����
	addr.sin_addr.s_addr = inet_addr("140.134.26.134");//10.22.25.21  173.1
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1999);
 
	//�]�w Listen
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);//SOMAXCONN: listening without any limit	
	
	//���ݳs�u
	
	CreateThread(NULL, 0,receive_cmds,(LPVOID)sListen, 0, &thread);
	//if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
	while(true)
	{
		//if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
		{
		for(;;) 
		{
			cap >> frame;
        
			Mat original = frame.clone();//�ƻs�ثe��frame
        
			Mat gray;
			cvtColor(original, gray, CV_BGR2GRAY);//�Nframe��Ƕ�

			vector< Rect_<int> > faces;//�bframe�M���y�P����
			vector< Rect_<int> > eye;

			//haar_cascade.detectMultiScale(gray, faces);
			//���ܰ����ؤo
			int fsz=50;
			haar_cascade.detectMultiScale( frame, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(fsz, fsz) );
			//                                          (     ��վ�~�P���Ѽ�        )
			if(faces.size>0)
			{
				writer.write(frame);
			}
			//�N�����쪺�y�����Ѩ����
			for(int i = 0; i < faces.size(); i++) 
			{
				//�M�Ű}�C�קK�z��
				t=i;
				box_text[i]='\0';
				box_text_2[i]='\0';
				message[i]='\0';

				Rect face_i = faces[i];// Process face by face:
				Mat face = gray(face_i);//�N�Ϥ��W���y���U��
				Mat face_resized;
			
				//���ܹϤ��j�p
				resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);
           
				//�إ߿��Ѫ��H�P�ۦ��ת��ܼ�
				int predictedLabel = -1;
				double confidence = 0.0;
				model->predict(face_resized, predictedLabel, confidence);

				  int prediction = model->predict(face_resized);
				//String prediction= model->predict(face_resized);
				//if(confidence>=24 && confidence<=30)

				rectangle(original, face_i, CV_RGB(0, 255,0), 1);//�e�X�˴��y�����x��

				if (confidence < 180)
				{
				//�C�L���Ѫ��H�P�ۦ���
				string box_text_3 = format("Prediction = %d, %f", predictedLabel, confidence);

				unsigned int x_axis = faces[i].x;//�إߩ�y�Ъ��ܼ�
				unsigned int y_axis = faces[i].y;

				unsigned int aa = faces[i].x;//�إߩ�ۦ��ת��ܼ�
				unsigned int bb = faces[i].y;

				//�p�G���ѵ��G���Y��
				switch (prediction)
				{
					case 0:
					box_text[i] = format("Bradley");//��ܦW�r
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
				box_text_2[i]=format("(%d,%d)",x_axis,y_axis);//��ܮy��
			
				//��ܦW�r����m  �ýT�{������~���ȩ�̭�
				int pos_x = max(face_i.tl().x + 10, 0);
				int pos_y = max(face_i.tl().y + 90, 0);

				//��ܮy�Ъ���m  �ýT�{������~���ȩ�̭�
				x_axis    = max(face_i.tl().x + 10, 0);
				y_axis    = max(face_i.tl().y + 105, 0);

				//��ܬۦ��ת���m
				aa    = max(face_i.tl().x + 10, 0);
				bb    = max(face_i.tl().y + 45, 0);

				//�N�W�����W�r�P�y�Щ�i�Ϥ�
				putText(original, box_text[i], Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
				putText(original, box_text_2[i], Point(x_axis,y_axis), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
			//	putText(original, box_text_3, Point(aa,bb), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
				int esz=1;
				eye_cascade.detectMultiScale(face, eye, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(esz,esz) );
				//�e�X��������ܮy��
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
				//��ܵ��G
				imshow("face_recognizer", original);

				//��������ݪ��T��
				char key = (char) waitKey(20);
				printf("%c",key);

				//�h�X�j��
				if(key == 27)//ASCII 27 ������esc
					break;
				//}
				//�{������20ms
				
			}
		}
	}
	}
    return 0;
}*/