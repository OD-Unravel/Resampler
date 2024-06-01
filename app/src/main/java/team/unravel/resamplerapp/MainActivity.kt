package team.unravel.resamplerapp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import team.unravel.resampler.Sonic

class MainActivity : AppCompatActivity() {
  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    setContentView(R.layout.activity_main)
    val sonic = Sonic(44100, 2)

    sonic.speed = 2f
    sonic.volume = 0.5f
    Log.e("SONIC","After setup ${sonic.speed} ${sonic.volume}")
  }
}