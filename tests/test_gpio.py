from pathlib import Path

def test_gpio_files_exist():
    assert Path("Drivers/Inc/gpio.h").exists()
    assert Path("Drivers/Src/gpio.c").exists()

def test_gpio_header_contains_api():
    text = Path("Drivers/Inc/gpio.h").read_text(encoding="utf-8")
    assert "GPIO_Init" in text
    assert "GPIO_WritePin" in text
    assert "GPIO_ReadPin" in text
    assert "GPIO_TogglePin" in text