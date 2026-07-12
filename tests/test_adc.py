from pathlib import Path

def test_adc_files_exist():
    assert Path("Drivers/Inc/adc.h").exists()
    assert Path("Drivers/Src/adc.c").exists()

def test_adc_header_contains_api():
    text = Path("Drivers/Inc/adc.h").read_text(encoding="utf-8")
    assert "ADC_Init" in text
    assert "ADC_Start" in text
    assert "ADC_Read" in text
    assert "ADC_Stop" in text