from pathlib import Path

def test_uart_files_exist():
    assert Path("Drivers/Inc/uart.h").exists()
    assert Path("Drivers/Src/uart.c").exists()

def test_uart_header_contains_api():
    text = Path("Drivers/Inc/uart.h").read_text(encoding="utf-8")
    assert "UART_Init" in text
    assert "UART_SendString" in text
    assert "UART_ReadByte" in text
    assert "UART_EnableInterrupt" in text