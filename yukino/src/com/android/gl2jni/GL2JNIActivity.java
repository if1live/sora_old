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


import android.os.Bundle;
import android.util.Log;
import android.widget.RelativeLayout;

import com.android.gl2jni.*;
import com.cauly.android.ad.AdInfo;
import com.cauly.android.ad.AdListener;
import com.cauly.android.ad.AdView;


public class GL2JNIActivity extends Activity implements AdListener {

	GL2JNIView mView;
	private RelativeLayout main_layout;

	@Override protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		
		setContentView(R.layout.test);
		main_layout = (RelativeLayout)findViewById(R.id.layout);
		//sora renderer는 나중에 부착하자
		mView = new GL2JNIView(getApplication());
		main_layout.addView(mView);

		AdInfo ads_info = new AdInfo();
		//String appcode = "Slek0KLF";
		String appcode = "CAULY";
		ads_info.initData(appcode, "CPC", "all", "all", "off", "default", "no", 90, false);
		AdView adView = new AdView(this);
		adView.setAdListener(this);
		main_layout.addView(adView);
	}

	@Override protected void onPause() {
		super.onPause();
		mView.onPause();
	}

	@Override protected void onResume() {
		super.onResume();
		mView.onResume();
	}

	public void onFailedToReceiveAd(boolean arg0) {
		// TODO Auto-generated method stub
		Log.w("yukino", "on failed");
	}

	public void onReceiveAd() {
		// TODO Auto-generated method stub
		Log.w("yukino", "on receive");
	}
}