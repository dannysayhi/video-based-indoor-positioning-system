package com.example.androidclient;
/*
 * 電腦學步玩:http://study2fun.blogspot.tw/
 */
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {

	static EditText ed;
	static TextView tv1,tv2;
	static Button btn ;
	static String str1="0",str2="0";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //先建立好與XML的元件連結
        ed = (EditText) findViewById(R.id.edt);
 //       tv1 = (TextView) findViewById(R.id.show01);
        tv2 = (TextView) findViewById(R.id.show02);
        btn = (Button) findViewById(R.id.btn);
        //撰寫一個按鈕事件，當按下按鈕時，才啟動一些功能
        btn.setOnClickListener(new OnClickListener() 
        {public void onClick(View arg0) 
		{
        	//建立 thread 的物件
        	Thread t = new thread();
            //啟動thread
        	t.start();
            try {
            	//先讓他等待個 3 秒
    			t.sleep(3000);
    		} catch (InterruptedException e) {}
            //之後在讓它顯示在銀幕上，有時網路不夠穩時，傳輸會較慢，而導致傳誦與接收都尚未完成，便顯示到銀幕上了
    //        tv1.setText(str1);
            tv2.setText(str2);
		}});    
    }
    class thread extends Thread{
    	public void run() {
    		try{
    		System.out.println("Waitting to connect......");
    		String server=ed.getText().toString();
    		int servPort=1050;
    		Socket socket=new Socket(server,servPort);
    		InputStream in=socket.getInputStream();
    		OutputStream out=socket.getOutputStream();
    		System.out.println("Connected!!");
    		byte[] rebyte = new byte[18];
    		in.read(rebyte);
    		str2 ="(Client端)接收的文字:"+ new String(new String(rebyte));
    		String str = "android client string";
    		str1 = "(Client端)傳送的文字:"+str;
    		byte[] sendstr = new byte[21];
    		System.arraycopy(str.getBytes(), 0, sendstr, 0, str.length());
    		out.write(sendstr);
    		}catch(Exception e)
    		{
    			System.out.println("Error: "+e.getMessage());
    		}
    		}
    }
}


