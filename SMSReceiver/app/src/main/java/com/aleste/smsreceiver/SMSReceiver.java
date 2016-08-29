package com.aleste.smsreceiver;

import android.content.BroadcastReceiver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.telephony.SmsMessage;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by aleste on 30/08/15.
 */
public class SMSReceiver extends BroadcastReceiver {
    String address;

    @Override
    public void onReceive(Context context, Intent intent) {

        Bundle extras = intent.getExtras();
        String messages = "";
        Log.i("SMSReceiver", "Recibí un SMS");
        if (extras != null){
            Object[] smsExtra = (Object[]) extras.get("pdus");

            for ( int i = 0; i < smsExtra.length; ++i )
            {
                SmsMessage sms = SmsMessage.createFromPdu((byte[]) smsExtra[i]);

                String body = sms.getMessageBody().toString();

                 address = sms.getOriginatingAddress();
if(body.equalsIgnoreCase("Machine pleine")){

   UpdateEtat updateEtat = new UpdateEtat();
    updateEtat.execute("PLEINE",address);
    Toast.makeText( context, "addr " + address, Toast.LENGTH_LONG ).show();
    //   http://localhost/CrudTrashCan/UpdateState.php?methode=updateState&numPuce=25156178&state=Pleine
    NotifyAdmin nt= new NotifyAdmin();
    nt.execute(address);

}else {
    //canuservalid 22111444 22

    String[] splited = body.split(" ");
    Toast.makeText( context, "longueur " + String.valueOf(splited.length), Toast.LENGTH_LONG ).show();
if(splited[0].equalsIgnoreCase("canuservalid") ){
int credit =  Integer.parseInt(splited[2])*20;

    SmsManager cansms = SmsManager.getDefault();
    cansms.sendTextMessage(splited[1], null, "You have fed "+ splited[2] + " cans according to that you have received "
                                            +String.valueOf(credit) +
                                            " points" + " Thank you for using Smart Trash Can", null, null);


    SmsManager trashsms = SmsManager.getDefault();
    trashsms.sendTextMessage(address, null,"OK", null, null);






}}
                messages += "SMS from " + address + " :\n";
                messages += body + "\n";

                //Log.i("SMSReceived", "SMS_BODY:"+sms.getMessageBody().toString());
                //Log.i("SMSReceived", "SMS_FROM:"+sms.getOriginatingAddress().toString());
            }
            Toast.makeText( context, messages, Toast.LENGTH_LONG ).show();
        }









    }





    public  class NotifyAdmin extends AsyncTask<String, Void, String> {




        protected String doInBackground(String... urls) {
            // String email = emailText.getText().toString();
            // Do some validation here
            String nummachine=urls[0];

            try {


                URL url = new URL("http://twasalni.com/CrudTrashCan/CrudMachine.php?methode=getNumAdminByNumMachine&numMachine="+nummachine);

                HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
                try {
                    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(urlConnection.getInputStream()));
                    StringBuilder stringBuilder = new StringBuilder();
                    String line;
                    while ((line = bufferedReader.readLine()) != null) {
                        stringBuilder.append(line).append("\n");
                    }
                    bufferedReader.close();
                    return stringBuilder.toString();
                }
                finally{
                    urlConnection.disconnect();
                }
            }
            catch(Exception e) {
                Log.e("ERROR", e.getMessage(), e);
                return null;
            }
        }

        protected void onPostExecute(String response) {
            if (response == "no") {
                response = "THERE WAS AN ERROR";
            } else {
                try {

                    JSONArray data = new JSONArray(response);


                        JSONObject admin = (JSONObject) data.get(0);
                        String numadmin=admin.getString("telephone");

                  //  String numadmin=userobj.getString("telephone");

                    SmsManager trashsms = SmsManager.getDefault();
                    trashsms.sendTextMessage(numadmin, null,"Machine number"+ address +"is Full ", null, null);










///                    adapter = new EvennementAdapter(getActivity(),
                    //                         R.layout.one_item, evento);
                    ///               listView.setAdapter(adapter);


                } catch (JSONException e) {
                    e.printStackTrace();
                }


            }

        }}




    public  class UpdateEtat extends AsyncTask<String, Void, String> {

        private Exception exception;


        protected void onPreExecute() {
            //  pDialog.show();

        }

        protected String doInBackground(String... parms) {
            // String email = emailText.getText().toString();
            // Do some validation here
            String state = parms[0];
            String num=parms[1];
            try {
                //   Toast.makeText(getContext(),urlJsonArry,Toast.LENGTH_SHORT).show();


                URL url = new URL( "http://twasalni.com/CrudTrashCan/UpdateState.php?methode=updateState&numPuce="
                        +
                        num
                        +
                        "&state="+
                        state
                        );
                HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
                try {
                    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(urlConnection.getInputStream()));
                    StringBuilder stringBuilder = new StringBuilder();
                    String line;
                    while ((line = bufferedReader.readLine()) != null) {
                        stringBuilder.append(line).append("\n");
                    }
                    bufferedReader.close();
                    return stringBuilder.toString();
                }
                finally{
                    urlConnection.disconnect();
                }
            }
            catch(Exception e) {
                Log.e("ERROR", e.getMessage(), e);
                return null;
            }
        }

        protected void onPostExecute(String response) {
            if (response == null) {
                response = "THERE WAS AN ERROR";
            } else {

                }



            }
            //     pDialog.hide();

        }


}





