package com.example.sameer.sendgetfile;

import android.app.Application;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 * Created by sameer on 8/3/16.
 */
public class FTPServerConnection implements Runnable {

    static public FTPFile[] mFileArray;
    public String ip, fileName;
    Context context;

    FTPServerConnection(String ip, String fileName, Context context) {
        this.ip = ip;
        this.fileName = fileName;
        this.context = context;
    }

    @Override
    public void run() {
        try {
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
                downloadAndUploadFile(mFtpClient);
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

    private void downloadAndUploadFile(FTPClient ftp) {
        for (FTPFile file : MainActivity.files) {
            if (!file.isFile()) {
                continue;
            }

            String currFileName = file.getName();
            Log.v("FILE NAME: ", currFileName);
            Log.v("REAL FILE NAME: ", this.fileName);
            if (currFileName.equals(this.fileName)) {
                Log.v("DOWNLOAD FILE: ", currFileName);
                try {
                    FileOutputStream fos = this.context.openFileOutput(currFileName, this.context.MODE_PRIVATE);
//                    OutputStream output = new FileOutputStream("/" + currFileName);
                    Log.v("FILE RERTREIVE: ", "Retreiving file from FTP server.");
                    ftp.retrieveFile(currFileName, fos);
                    fos.close();

                    // TODO: upload a file from the tablet onto FTP
                } catch (FileNotFoundException e) {
                    Log.e("FILE NOT FOUND:", currFileName);
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
