// 조도센서를 아날로그 A0핀으로 설정합니다.
int cds = A0;
// LED를 디지털 13핀으로 설정합니다.
int led = 13;

void setup() {
  // 조도센서의 동작 상태를 확인하기 위하여 시리얼 통신을 설정합니다. (전송속도 9600bps)
  Serial.begin(9600);
  // LED 핀을 OUTPUT으로 설정합니다.
  pinMode(led, OUTPUT);
}

void loop() {
  // 조도센서로 부터 측정된 밝기 값을 읽습니다.
  // 조도센서로 부터 입력되어지는 전압의 크기 (0~5V)에 따라 0~1023 범위의 값으로 변환되어 반환합니다.
  int cdsValue = analogRead(cds);

  // 측정된 밝기 값를 시리얼 모니터에 출력합니다.
  Serial.print("cds =  ");
  Serial.println(cdsValue);

  // 조도센서로 부터 측정된 밝기 값이 500보다 크다면, 아래의 블록을 실행합니다.
  if (cdsValue > 500) {
    // LED가 연결된 핀의 로직레벨을 HIGH (5V)로 설정하여, LED가 켜지도록 합니다.
    digitalWrite(led, HIGH);
    // LED상태를 시리얼 모니터에 출력합니다.
    Serial.println("LED ON (cds > 500)");
  }
  // 조도센서로 부터 측정된 밝기 값이 500보다 작면, 아래의 블록을 실행합니다.
  else {
    // LED가 연결된 핀의 로직레벨을 LOW (0V)로 설정하여, LED가 꺼지도록 합니다.
    digitalWrite(led, LOW);
    // LED상태를 시리얼 모니터에 출력합니다.
    Serial.println("LED OFF (cds < 500)");
  }
  // 0.2초 동안 대기합니다.
  delay(200);
}
