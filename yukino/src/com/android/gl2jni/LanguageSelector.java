package com.android.gl2jni;

import android.app.Activity;

import android.graphics.drawable.Drawable;
import android.widget.ImageButton;
import android.widget.LinearLayout;

public class LanguageSelector {
	private static LanguageSelector ctx = null;
	public static LanguageSelector GetInstance() {
		if(ctx == null) {
			ctx = new LanguageSelector();
		}
		return ctx;
	}
	public LanguageSelector() {
		//로케일에 따라서 기초 언어 설정하기위해서 btn수정이 필요하니
		//main같은 부분에서 잘 처리하자
		currLang = Undefined;
	}
	
	public static final int Undefined = 0x00;
	public static final int Korean = 0x01;
	public static final int English = 0x02;
	
	public void SetLanguage(int code, Activity activity) {
		currLang = code;
		ResetUI(activity);
	}
	public void ResetUI(Activity activity) {
		//언어에 따라서 배경 이미지 교체
		//언어에 따라서 button 이미지 교체
		ImageButton korBtn = (ImageButton)activity.findViewById(R.id.LangKoreanButton);
		ImageButton engBtn = (ImageButton)activity.findViewById(R.id.LangEnglishButton);
		if(korBtn != null && engBtn != null) {
			if(currLang == Korean) {
				korBtn.setSelected(true);
				engBtn.setSelected(false);
				
				LinearLayout layout = (LinearLayout)activity.findViewById(R.id.IntroLayout);
				int resId = R.drawable.intro_menu_k;
				Drawable drawable = activity.getResources().getDrawable(resId);
				layout.setBackgroundDrawable(drawable);
				
			} else {
				korBtn.setSelected(false);
				engBtn.setSelected(true);
				
				LinearLayout layout = (LinearLayout)activity.findViewById(R.id.IntroLayout);
				int resId = R.drawable.intro_menu_e;
				Drawable drawable = activity.getResources().getDrawable(resId);
				layout.setBackgroundDrawable(drawable);
			}
		}
	}
	public int GetCurrLanguage() {
		return currLang;
	}
	private int currLang;
}
