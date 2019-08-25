package com.example.unknown.bluetooth_led;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;


import java.io.IOException;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private final int REQUEST_BLUETOOTH_ENABLE = 100;
    BluetoothSocket btSocket = null;
    BluetoothDevice mBluetoothDevice = null;

    static BluetoothAdapter mBluetoothAdapter;

    private ArrayAdapter<String> mConversationArrayAdapter;

    static boolean isConnectionError = false;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ListView mMessageListview = (ListView) findViewById(R.id.message_listview);

        mConversationArrayAdapter = new ArrayAdapter<>( this,
                android.R.layout.simple_list_item_1 );
        mMessageListview.setAdapter(mConversationArrayAdapter);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            showErrorDialog("This device is not implement Bluetooth.");
            return;
        }

        if (!mBluetoothAdapter.isEnabled()) {
            Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(intent, REQUEST_BLUETOOTH_ENABLE);
        }
        else {
            showPairedDevicesListDialog();
        }

        Button sendButton = (Button) findViewById(R.id.btn);
        sendButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    btSocket.getOutputStream().write("1".toString().getBytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private class ConnectTask extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        ConnectTask(BluetoothDevice bluetoothDevice) {
            mBluetoothDevice = bluetoothDevice;
        }

        @Override
        protected void onPreExecute()
        {
            //progress = ProgressDialog.show(ledControl.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isConnectionError)
                {
                    //SPP
                    UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
                    btSocket=mBluetoothDevice.createRfcommSocketToServiceRecord(uuid);
                    btSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                //msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            }
            else
            {
                //msg("Connected.");
                isConnectionError = true;
            }
            //progress.dismiss();
        }
    }

    public void showPairedDevicesListDialog()
    {
        Set<BluetoothDevice> devices = mBluetoothAdapter.getBondedDevices();
        final BluetoothDevice[] pairedDevices = devices.toArray(new BluetoothDevice[0]);

        if ( pairedDevices.length == 0 ){
            showQuitDialog( "No devices have been paired.\n"
                    +"You must pair it with another device.");
            return;
        }

        String[] items;
        items = new String[pairedDevices.length];
        for (int i=0;i<pairedDevices.length;i++) {
            items[i] = pairedDevices[i].getName();
        }

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Select device");
        builder.setCancelable(false);
        builder.setItems(items, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();

                ConnectTask task = new ConnectTask(pairedDevices[which]);
                task.execute();
            }
        });
        builder.create().show();
    }

    public void showErrorDialog(String message)
    {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Quit");
        builder.setCancelable(false);
        builder.setMessage(message);
        builder.setPositiveButton("OK",  new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                if ( isConnectionError  ) {
                    isConnectionError = false;
                    finish();
                }
            }
        });
        builder.create().show();
    }

    public void showQuitDialog(String message)
    {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Quit");
        builder.setCancelable(false);
        builder.setMessage(message);
        builder.setPositiveButton("OK",  new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                finish();
            }
        });
        builder.create().show();
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        if(requestCode == REQUEST_BLUETOOTH_ENABLE){
            if (resultCode == RESULT_OK){
                //BlueTooth is now Enabled
                showPairedDevicesListDialog();
            }
            if(resultCode == RESULT_CANCELED){
                showQuitDialog( "You need to enable bluetooth");
            }
        }
    }
}
