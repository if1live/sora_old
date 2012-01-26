package com.android.gl2jni;

import android.content.Context;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

public class SoraGyro implements SensorEventListener {
	
	private SensorManager sensorMgr;
	private Sensor gyro;
	private Context context;

	public SoraGyro(Context ctx) {
		context = ctx;
		//센서 먼저 활성화하기
		sensorMgr = (SensorManager)ctx.getSystemService(Context.SENSOR_SERVICE);
		gyro = sensorMgr.getDefaultSensor(Sensor.TYPE_ORIENTATION);
	}
	public void enable() {
		sensorMgr.registerListener(this, gyro, SensorManager.SENSOR_DELAY_GAME);
	}
	public void disable() {
		sensorMgr.unregisterListener(this);
	}
	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub
		
	}

	protected float[] accelVals;
	
	public void onSensorChanged(SensorEvent event) {
		
		//if (event.sensor.getType() != Sensor.TYPE_ORIENTATION){
		//return;
		//}
		
		//accelVals = lowPass( event.values, accelVals );
		
		//원본데이터가 deg인데 rad로 변환해야한다
		
		//float yaw = accelVals[0] * 3.141592f / 180.0f;
		//float pitch = accelVals[1] * 3.141592f / 180.0f;
		//float roll = accelVals[2] * 3.141592f / 180.0f;
		//GL2JNILib.addGryo(yaw, pitch, roll);
		
		float yaw = event.values[0] * 3.141592f / 180.0f;
		float pitch = event.values[1] * 3.141592f / 180.0f;
		float roll = event.values[2] * 3.141592f / 180.0f;
		
		//세로잡기에서는 축 교체해야됨
		int orientation = context.getResources().getConfiguration().orientation;
		if (orientation == Configuration.ORIENTATION_LANDSCAPE){
			float tmp = yaw;
			yaw = -pitch;
			pitch = tmp;
		}
		
		GL2JNILib.addGryo(yaw, pitch, roll);
		
		//Log.d("a", "x = " + event.values[0] + " y = " + event.values[1] + " z = " + event.values[2]);
		//Log.d("a", "x = " + accelVals[0] + " y = " + accelVals[1] + " z = " + accelVals[2]);
	}
	
	//http://stackoverflow.com/questions/4611599/help-smoothing-data-from-a-sensor/4611772#4611772
	static final float ALPHA = 0.2f;
	/**
	 * @see http://en.wikipedia.org/wiki/Low-pass_filter#Algorithmic_implementation
	 * @see http://developer.android.com/reference/android/hardware/Sensor.html#TYPE_ACCELEROMETER
	 */
	protected float[] lowPass( float[] input, float[] output ) {
	    if ( output == null ) return input;

	    for ( int i=0; i<input.length; i++ ) {
	        output[i] = output[i] + ALPHA * (input[i] - output[i]);
	    }
	    return output;
	}
	public boolean isAvaliable() {
		if(gyro == null) {
			return false;
		} else {
			return true;
		}
	}

}
