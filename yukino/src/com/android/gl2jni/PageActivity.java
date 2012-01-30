package com.android.gl2jni;

import java.util.Vector;

import kr.co.teamobj.snowwhite.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RadioGroup.OnCheckedChangeListener;

public class PageActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.page);
		
		//현재 언어를 얻어서 radio button 설정
		int langCode = LanguageSelector.GetInstance().GetCurrLanguage();
		RadioButton engBtn = (RadioButton)findViewById(R.id.engRadioButton);
		RadioButton korBtn = (RadioButton)findViewById(R.id.korRadioButton);
		if(langCode == LanguageSelector.Korean) {
			engBtn.setChecked(false);
			korBtn.setChecked(true);
		} else {
			engBtn.setChecked(true);
			korBtn.setChecked(false);
		}
		
		//라디오 버튼 언어 연결
		final Activity activity = this;
		RadioGroup langGroup = (RadioGroup)findViewById(R.id.langRadioGroup);
		langGroup.setOnCheckedChangeListener(new OnCheckedChangeListener() {
			public void onCheckedChanged(RadioGroup group, int checkedId) {
				if(checkedId == R.id.engRadioButton) {
					LanguageSelector.GetInstance().SetLanguage(LanguageSelector.English, activity);
				} else if(checkedId == R.id.korRadioButton) {
					LanguageSelector.GetInstance().SetLanguage(LanguageSelector.Korean, activity);
				}
			}
		});
		
		//listview에 내용 적절히 채우기
		
		ListView sceneListView = (ListView)findViewById(R.id.sceneListView);
		sceneListView.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, sceneList));
		sceneListView.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> l, View v, int position, long id) {
				int scenePageIndex = position * 2;	//script * raw
				Log.i("fds", "fds");
				//TODO NDK
				
				//이전에 생성된것이 하나도 없으면 그냥 만들기
				//Intent intentSubActivity = new Intent(activity, SceneActivity.class);
				//startActivity(intentSubActivity);
				
				//이전에 게임씬을 거쳐서 왔으면 finish
				finish();
			}
		});
	}
	
	final int sceneCount = 10;
	static final String[] sceneList = new String[] {
		"Scene #1",
		"Scene #2",
		"Scene #3",
		"Scene #4",
		"Scene #5",
		"Scene #6",
		"Scene #7",
		"Scene #8",
		"Scene #9",
		"Scene #10",
	};

	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		super.onBackPressed();
		finish();
	}
}
