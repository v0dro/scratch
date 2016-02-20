package com.example.user1.helloworld;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.TextView;
import java.lang.Math;

public class MainActivity extends AppCompatActivity {

    public double total;
    public String lastBinaryOp, lastUnaryOp;
    public boolean equalsOp, firstTimeNumPad, opPressed;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView d = (TextView) findViewById(R.id.main_text);
        d.setText("");
        total = 0.0;
        lastBinaryOp = "";
        lastUnaryOp = "";
        equalsOp = false;
        opPressed = false;
        firstTimeNumPad = true;
    }

    public void numberFunction(View view) {
        TextView display = (TextView) findViewById(R.id.main_text);
        String text = "bro";

        if (firstTimeNumPad) {
            display.setText("");
        }

        switch (view.getId()) {
            case R.id.zero:   text = "0"; break;
            case R.id.one:    text = "1"; break;
            case R.id.two:    text = "2"; break;
            case R.id.three:  text = "3"; break;
            case R.id.four:   text = "4"; break;
            case R.id.five:   text = "5"; break;
            case R.id.six:    text = "6"; break;
            case R.id.seven:  text = "7"; break;
            case R.id.eight:  text = "8"; break;
            case R.id.nine:   text = "9"; break;
        }
        firstTimeNumPad = false;

        display.setText(display.getText() + text);
    }

    public void masterFunction(View view) {
        TextView display = (TextView) findViewById(R.id.main_text);
        double currNum = Double.parseDouble(display.getText().toString());

        if (view.getId() == R.id.equals) {
            equalsOp = true;
        }

        if ((lastBinaryOp != "" && opPressed) || equalsOp) {
            switch (lastBinaryOp) {
                case "+":
                    total += currNum; break;
                case "-":
                    total -= currNum; break;
                case "*":
                    total *= currNum; break;
                case "/":
                    total /= currNum; break;
            }

            lastBinaryOp = "";
            if (opPressed) {
                opPressed = false;
            }
            if (equalsOp) {
                equalsOp = false;
            }
        }

        switch (view.getId()) {
            case R.id.plus:     lastBinaryOp = "+"; break;
            case R.id.minus:    lastBinaryOp = "-"; break;
            case R.id.multiply: lastBinaryOp = "*"; break;
            case R.id.divide:   lastBinaryOp = "/"; break;
            case R.id.sin: lastUnaryOp = "sin"; break;
            case R.id.cos: lastUnaryOp = "cos"; break;
            case R.id.tan: lastUnaryOp = "tan"; break;
        }

        if (lastUnaryOp != "") {
            switch (lastUnaryOp) {
                case "sin":
                    total = Math.sin(currNum);
                    break;
                case "cos":
                    total = Math.cos(currNum);
                    break;
                case "tan":
                    total = Math.tan(currNum);
                    break;
            }

            lastUnaryOp = "";
        }

        if (opPressed == false) {
            opPressed = true;
        }

        firstTimeNumPad = true;
        if (total == 0.0) {
            total = currNum;
        }
        display.setText(String.valueOf(total));
    }

    public void clearFunction(View view) {
        total = 0.0;

        TextView display = (TextView) findViewById(R.id.main_text);
        display.setText("");
    }
}
