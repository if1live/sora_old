package kr.co.teamobj.snowwhite;

import com.android.gl2jni.IntroActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

public class SplashActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	     setContentView(R.layout.splash);
	     
	     final Activity activity = this;
	     
	     Handler handler = new Handler() {
	    	 @Override
	    	public void handleMessage(Message msg) {
	    		finish();
	    		
	    		Intent intentSubActivity = new Intent(activity, IntroActivity.class);
				startActivity(intentSubActivity);
	    	}
	     };
	     handler.sendEmptyMessageDelayed(0, 1500);
	}
}
