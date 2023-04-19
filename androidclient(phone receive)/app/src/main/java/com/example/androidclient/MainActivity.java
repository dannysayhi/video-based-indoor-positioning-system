package com.example.androidclient;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.DataInputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Arrays;

public class MainActivity extends Activity {

	static EditText ed,ed_2;
	static TextView tv2;
	static Button btn ;
	static String str2="0";

	private Handler mHandler = new Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
			switch(msg.what){
				case 0:
					tv2.setText(str2);//之後在讓它顯示在銀幕上
					break;
			}
			super.handleMessage(msg);
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		//先建立好與XML的元件連結
		ed = (EditText) findViewById(R.id.edt);
		tv2 = (TextView) findViewById(R.id.show02);
		btn = (Button) findViewById(R.id.btn);

		//撰寫一個按鈕事件，當按下按鈕時，才啟動一些功能
		btn.setOnClickListener(new OnClickListener()
							   {
								   public void onClick(View arg0)
								   {
									   //建立 thread 的物件
									   Thread t = new thread();

									   //啟動thread
									   t.start();
									   try {
										   //有時網路不夠穩時，傳輸會較慢，而導致傳誦與接收都尚未完成，便顯示到銀幕上，所以先讓他等待個 3 秒
										   t.sleep(3000);
									   } catch (InterruptedException e) {
										   System.out.println("");
									   }
								   }
							   }
		);
	}
	class thread extends Thread{
		public void run()
		{
			Socket socket;

			try {
				System.out.println("Waitting to connect......");
				String server = ed.getText().toString();
				String name = "Ellen";

				int servPort = 1999;
				socket = new Socket(server, servPort);
				InputStream in = socket.getInputStream();
				OutputStream out = socket.getOutputStream();
				DataInputStream dis = new DataInputStream(in);

				int readed = 0;
				while (true)
				{
					System.out.println("Connected!!");

					byte[] sendstr = new byte[300];
					System.arraycopy(name.getBytes(), 0, sendstr, 0, name.length());
					out.write(sendstr);
					System.out.println(sendstr);

					byte[] rebyte = new byte[1024];
					readed = dis.read(rebyte);
					str2 =new String(rebyte);//將rebyte全部顯示出來 空的部分會亂碼
					str2 = str2.substring(0,readed);//將rebyte的內容顯示出來 空的部分不會顯示
					str2 = str2.substring(0,str2.indexOf(")")+1);//將rebyte ")" 之前的內容顯示出來 後面的部分不會顯示
					Arrays.fill(rebyte, (byte) 0);//將rebyte陣列的值全部變0
					System.out.println(str2 + "\n" + rebyte.length + "\n" + readed);

					Message msg = new Message();
					msg.what = 0;
					mHandler.sendMessage(msg);
				}
			}
			catch(Exception e)
			{
				System.out.println("Error: " + e.getMessage());
			}
		}
	}
}