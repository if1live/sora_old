package com.android.gl2jni;

import android.content.Context;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

public class SoraAccelerometer implements SensorEventListener {

	private SensorManager sensorMgr;
	private Context context;

	public SoraAccelerometer(Context ctx) {
		context = ctx;
		//센서 먼저 활성화하기
		sensorMgr = (SensorManager)ctx.getSystemService(Context.SENSOR_SERVICE);
	}
	public void enable() {
		sensorMgr.registerListener(this, 
				sensorMgr.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_NORMAL);
		//sensorMgr.registerListener(this, 
		//		sensorMgr.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD), SensorManager.SENSOR_DELAY_NORMAL);
	}
	public void disable() {
		sensorMgr.unregisterListener(this);
	}

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		// TODO Auto-generated method stub

	}
	
	public void onSensorChanged(SensorEvent event) {
		if (event.sensor.getType() != Sensor.TYPE_ACCELEROMETER){
            return;
		}

		//float x = event.values[0];
		//float y = event.values[1];
		//float z = event.values[2];
		accelVals = lowPass( event.values, accelVals );
		float x = accelVals[0];
		float y = accelVals[1];
		float z = accelVals[2];
		
		// Because the axes are not swapped when the device's screen orientation changes. 
		//So we should swap it here.
		int orientation = context.getResources().getConfiguration().orientation;
		if (orientation == Configuration.ORIENTATION_LANDSCAPE){
			float tmp = x;
			x = -y;
			y = tmp;
		}

		GL2JNILib.addAccel(x, y, z);
        //onSensorChanged(x, y, z, event.timestamp);
        //Log.d("a", "x = " + event.values[0] + " y = " + event.values[1] + " z = " + event.values[2]);   
	}

	protected float[] accelVals;
	//http://stackoverflow.com/questions/4611599/help-smoothing-data-from-a-sensor/4611772#4611772
	static final float ALPHA = 0.2f;
	//@see http://en.wikipedia.org/wiki/Low-pass_filter#Algorithmic_implementation
	//@see http://developer.android.com/reference/android/hardware/Sensor.html#TYPE_ACCELEROMETER
	protected float[] lowPass( float[] input, float[] output ) {
	    if ( output == null ) return input;

	    for ( int i=0; i<input.length; i++ ) {
	        output[i] = output[i] + ALPHA * (input[i] - output[i]);
	    }
	    return output;
	}
}
