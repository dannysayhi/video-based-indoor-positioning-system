package com.example.androidclient;
/*
 * �q���ǨB��:http://study2fun.blogspot.tw/
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
        //���إߦn�PXML������s��
        ed = (EditText) findViewById(R.id.edt);
 //       tv1 = (TextView) findViewById(R.id.show01);
        tv2 = (TextView) findViewById(R.id.show02);
        btn = (Button) findViewById(R.id.btn);
        //���g�@�ӫ��s�ƥ�A����U���s�ɡA�~�Ұʤ@�ǥ\��
        btn.setOnClickListener(new OnClickListener() 
        {public void onClick(View arg0) 
		{
        	//�إ� thread ������
        	Thread t = new thread();
            //�Ұ�thread
        	t.start();
            try {
            	//�����L���ݭ� 3 ��
    			t.sleep(3000);
    		} catch (InterruptedException e) {}
            //����b������ܦb�ȹ��W�A���ɺ�������í�ɡA�ǿ�|���C�A�ӾɭP�ǻw�P�������|�������A�K��ܨ�ȹ��W�F
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
    		str2 ="(Client��)��������r:"+ new String(new String(rebyte));
    		String str = "android client string";
    		str1 = "(Client��)�ǰe����r:"+str;
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


