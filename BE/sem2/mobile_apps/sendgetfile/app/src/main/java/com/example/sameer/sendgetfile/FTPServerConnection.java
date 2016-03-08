package com.example.sameer.sendgetfile;

import android.os.AsyncTask;
import android.util.Log;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;

import java.io.IOException;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 * Created by sameer on 8/3/16.
 */
public class FTPServerConnection implements Runnable {

    static public FTPFile[] mFileArray;
    public String ip;

    FTPServerConnection(String ip) {
        this.ip = ip;
    }

    @Override
    public void run() {
        boolean status = false;
        try {
            Log.e("SIMPLE", "3");
            FTPClient mFtpClient = new FTPClient();
            mFtpClient.setConnectTimeout(10 * 1000);
            mFtpClient.connect(this.ip);
            Log.e("Reply: ", mFtpClient.getReplyString());
            String username = "anonymous";
            String password = System.getProperty("user.name")+"@"+ InetAddress.getLocalHost().getHostName();
            mFtpClient.login(username, password);
            if (FTPReply.isPositiveCompletion(mFtpClient.getReplyCode())) {
                mFtpClient.setFileType(FTP.BINARY_FILE_TYPE);
                mFtpClient.enterLocalPassiveMode();
                MainActivity.files = mFtpClient.listFiles();
                status = true;
            }
        } catch (SocketException e) {
            Log.e("SocketException", "SocketException");
            e.printStackTrace();
        } catch (UnknownHostException e) {
            Log.e("UnknownHostException", "UnknownHostException");
            e.printStackTrace();
        } catch (IOException e) {
            Log.e("IOException", e.getMessage());
        }
    }
}
