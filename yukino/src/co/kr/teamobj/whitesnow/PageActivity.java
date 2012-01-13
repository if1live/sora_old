package co.kr.teamobj.whitesnow;

import com.android.gl2jni.R;

import android.app.Activity;
import android.os.Bundle;
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
		//final int scene = 10;
		//ListView sceneListView = (ListView)findViewById(R.id);
		//sceneListView.add
	}
	
	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		super.onBackPressed();
		finish();
	}
}
