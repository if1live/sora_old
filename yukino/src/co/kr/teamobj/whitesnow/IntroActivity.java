package co.kr.teamobj.whitesnow;

import java.util.Locale;

import com.android.gl2jni.R;

import android.app.Activity;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;

public class IntroActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.intro);
		
		final Activity activity = this;
		
		//현재 로케일을 얻어서 button 상태 초기화
		Locale locale = Locale.getDefault();
		if(locale == Locale.KOREAN) {
			LanguageSelector.GetInstance().SetLanguage(LanguageSelector.Korean, this);
		} else {
			LanguageSelector.GetInstance().SetLanguage(LanguageSelector.English, this);
		}
		
		//bgm 초기화
		//bgm이 들어가면 용량이 확 늘어나서 제외했다
		//MediaPlayer player = MediaPlayer.create(this, R.raw.bgm);
		//BGMPlayer.GetInstance().Init(player);
		
		//한쿡어 버튼
		ImageButton korBtn = (ImageButton)findViewById(R.id.LangKoreanButton);
		korBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				LanguageSelector.GetInstance().SetLanguage(LanguageSelector.Korean, activity);
			}
		});
		
		
		//영어 버튼
		ImageButton engBtn = (ImageButton)findViewById(R.id.LangEnglishButton);
		engBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				LanguageSelector.GetInstance().SetLanguage(LanguageSelector.English, activity);
			}
		});
		
		//시작 버튼
		ImageButton startBtn = (ImageButton)findViewById(R.id.StartButton);
		startBtn.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
			}
		});
		
		//페이지 버튼
		ImageButton pageBtn = (ImageButton)findViewById(R.id.PageButton);
		pageBtn.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
			}
		});
		
		/*
		//음악 버튼
		ImageButton bgmBtn = (ImageButton)findViewById(R.id.BGMButton);
		bgmBtn.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				// TODO Auto-generated method stub
				ImageButton bgmBtn = (ImageButton)findViewById(R.id.BGMButton);
				boolean isPlaying = BGMPlayer.GetInstance().IsPlay();
				if(isPlaying == true) {
					BGMPlayer.GetInstance().Stop();
					bgmBtn.setSelected(false);
				} else {
					BGMPlayer.GetInstance().Play();
					bgmBtn.setSelected(true);
				}
			}
		});
		*/
	}
}
