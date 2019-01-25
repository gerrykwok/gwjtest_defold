package com.xishanju.plm.misc;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.content.Intent;
import android.util.Log;

@SuppressLint("NewApi")
public class RequestPermissionActivity extends Activity
{
	int m_countResume;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		m_countResume = 0;
	}
	@Override
	protected void onResume()
	{
		super.onResume();
		m_countResume++;

		if(m_countResume == 1)
		{
			Intent iii = getIntent();
			String[] permissions = iii.getStringArrayExtra("permissions");
			this.requestPermissions(permissions, 123);
		}
	}
	@Override
	protected void onPause()
	{
		super.onPause();
	}
	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
	{
		super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		Log.d(MiscFunc.TAG, "onRequestPermissionsResult,size=" + permissions.length);
		MiscFunc.notifyLuaOfRequestPermissions(permissions, grantResults);
		finish();
	}
}
