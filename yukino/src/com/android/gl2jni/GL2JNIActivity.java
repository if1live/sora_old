/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gl2jni;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorManager;


import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import com.android.gl2jni.*;
import com.android.gl2jni.R.id;
import com.cauly.android.ad.AdInfo;
import com.cauly.android.ad.AdListener;
import com.cauly.android.ad.AdView;


public class GL2JNIActivity extends Activity implements AdListener {

	GL2JNIView mView = null;
	private RelativeLayout main_layout;
	
	private SoraAccelerometer accelerometer;
	private SoraGyro gyro;
	
	@Override protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		
		//가속도 센서 생성
		gyro = new SoraGyro(this);
		if(gyro.isAvaliable() == true) {
			GL2JNILib.useGyro();
			gyro.enable();
			Log.d("asd", "use gyro");
		} else {
			accelerometer = new SoraAccelerometer(this);
			GL2JNILib.useAccel();
			accelerometer.enable();
			Log.d("asd", "use accel");
		}
		
		setContentView(R.layout.scene);
		main_layout = (RelativeLayout)findViewById(R.id.layout);
		//sora renderer는 나중에 부착하자
		mView = new GL2JNIView(getApplication());
		main_layout.addView(mView, 0);

		/*
		 광고는 일단 사용하지 않는다. 결제수단을 달 생각이 아직 없어서.
		AdInfo ads_info = new AdInfo();
		//String appcode = "Slek0KLF";
		String appcode = "CAULY";
		ads_info.initData(appcode, "CPC", "all", "all", "off", "default", "no", 90, false);
		AdView adView = new AdView(this);
		adView.setAdListener(this);
		LinearLayout contentLayout = (LinearLayout)findViewById(R.id.contentLayout);
		contentLayout.addView(adView, 0);
		*/
		final Activity activity = this;
		//버튼에 이벤트 걸기
		ImageButton pageBtn = (ImageButton)findViewById(R.id.pageBtn);
		pageBtn.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				Intent intentSubActivity = new Intent(activity, PageActivity.class);
				startActivity(intentSubActivity);	//그리고 메뉴씬으로
			}
		});
		final ImageButton nextBtn = (ImageButton)findViewById(R.id.nextBtn);
		final ImageButton prevBtn = (ImageButton)findViewById(R.id.prevBtn);
		nextBtn.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				GL2JNILib.moveNextPage();
				/*
				//prev, next 버튼 갱신
				if(GL2JNILib.isNextPageExist() == false) {
					nextBtn.setAlpha(0);
					nextBtn.setEnabled(false);
				} else {
					nextBtn.setAlpha(255);
					nextBtn.setEnabled(true);
				}
				if(GL2JNILib.isPrevPageExist() == true) {
					prevBtn.setAlpha(255);
					prevBtn.setEnabled(true);
				} else {
					prevBtn.setAlpha(0);
					prevBtn.setEnabled(false);
				}
				*/
			}
		});
		prevBtn.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				GL2JNILib.movePrevPage();
				/*
				//prev, next 버튼 갱신
				if(GL2JNILib.isNextPageExist() == false) {
					nextBtn.setAlpha(0);
					nextBtn.setEnabled(false);
				} else {
					nextBtn.setAlpha(255);
					nextBtn.setEnabled(true);
				}
				if(GL2JNILib.isPrevPageExist() == true) {
					prevBtn.setAlpha(255);
					prevBtn.setEnabled(true);
				} else {
					prevBtn.setAlpha(0);
					prevBtn.setEnabled(false);
				}
				*/
			}
		});
		ImageButton resetBtn = (ImageButton)findViewById(R.id.resetBtn);
		resetBtn.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// TODO 자이로인지 가속도인지 확인해서 적절한 값을 보내자
				GL2JNILib.resetAccel();
			}
		});
	}

	@Override protected void onPause() {
		//정지지원 제거. gl과 내장ui가 충돌하기떄문에 그냥끄게했다
		System.exit(0);
		super.onPause();
		if(mView != null) {
			mView.onPause();
			
			((RelativeLayout)mView.getParent()).removeAllViewsInLayout();
			//((RelativeLayout)mView.getParent()).removeView(mView);
			mView = null;
		}		
	}

	@Override protected void onResume() {
		super.onResume();
		if(mView != null) {
			mView.onResume();
		}
		else {
			setContentView(R.layout.scene);
			main_layout = (RelativeLayout)findViewById(R.id.layout);
			//sora renderer는 나중에 부착하자
			mView = new GL2JNIView(getApplication());
			main_layout.addView(mView, 0);
		}
		
	}
	

	public void onFailedToReceiveAd(boolean arg0) {
		// TODO Auto-generated method stub
		Log.w("yukino", "on failed");
	}

	public void onReceiveAd() {
		// TODO Auto-generated method stub
		Log.w("yukino", "on receive");
	}
	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		super.onBackPressed();
		finish();
		
		System.exit(0);
	}
}