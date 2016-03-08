package com.example.sameer.sendgetfile;

import android.content.DialogInterface;
import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;

import java.io.IOException;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {
    String ipAddress;
    static public FTPFile[] files;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button connect = (Button) findViewById(R.id.connectButton);

        connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EditText ip = (EditText) findViewById(R.id.ipAddressText);
                String ipAddress = ip.getText().toString();
                Log.v("START: ", "starting server");
                Thread f = new Thread(new FTPServerConnection(ipAddress));
                f.start();
                try {
                    f.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Log.v("LENGTH: ", String.valueOf(files.length));
            }
        });
    }
}

