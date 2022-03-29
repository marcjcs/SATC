package com.marcjcs.smex1

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.EditText
import android.widget.TextView
import kotlin.math.pow

class BMI : AppCompatActivity() {
    private lateinit var height:EditText
    private lateinit var weight:EditText
    private lateinit var bmiResult:TextView
    private lateinit var bmiCondition:TextView
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_bmi)
        height = findViewById<EditText>(R.id.editTextPersonHeight)
        weight = findViewById<EditText>(R.id.editTextPersonWeight)
        bmiResult = findViewById<TextView>(R.id.textViewBMIResult)
        bmiCondition = findViewById<TextView>(R.id.textViewBMICondition)
    }

    fun onCalculate(view: View) {
        val personWeight = weight.text.toString().toDouble()
        val personHeight = height.text.toString().toDouble()
        val result = personWeight / personHeight.pow(2.0)
        val roundedResult = String.format("%.2f", result)
        bmiResult.text = "Your BMI is: $roundedResult"

        if (result < 16) bmiCondition.text = "Severe Thinness"
        if (result >= 16 && result < 17) bmiCondition.text = "Moderate Thinness"
        if (result >= 17 && result < 18.5) bmiCondition.text = "Mild Thinness"
        if (result >= 18.5 && result < 25) bmiCondition.text = "Normal"
        if (result >= 25 && result < 30) bmiCondition.text = "Overweight"
        if (result >= 30 && result < 35) bmiCondition.text = "Obese I"
        if (result >= 35 && result < 40) bmiCondition.text = "Obese II"
        if (result >= 40) bmiCondition.text = "Obese III"
    }
}