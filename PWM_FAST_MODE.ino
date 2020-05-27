#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define _BV(bit) (1 << (bit))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))

uint8_t PWM_RED_PIN = PD3;
uint8_t PWM_GREEN_PIN = PD5;
uint8_t PWM_BLUE_PIN = PB2;

uint8_t LED_BLUE_PIN = PD7;
uint8_t LED_GREEN_PIN = PD4;

uint8_t BUTTON_PIN = PB0;

uint8_t POTENTIOMETER_PIN = PC0;

uint8_t PWM_RED_VALUE = 0;
uint8_t PWM_GREEN_VALUE = 0;
uint8_t PWM_BLUE_VALUE = 0;

uint8_t SELECTED;

uint8_t count = 0;
bool filter = 0;

void adc_init()
{
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  cbi(ADCSRA, ADATE);
  sbi(ADCSRA, ADEN);

  sbi(ADMUX, REFS0);
  sbi(ADMUX, ADLAR);
}


uint16_t adc_read(uint8_t pin)
{
  int channel = pin & 0x07;
  ADMUX |= channel;
  sbi(ADCSRA, ADSC);
  sbi(ADMUX, ADLAR);
  while (bit_is_set(ADCSRA, ADIF));
  return ADCH;
}

void adc_write(uint8_t pin, uint8_t value, uint32_t frequency)
{
  if (frequency > 1000000 || frequency < 62) return; // Returns if the frequency is not valid
  uint8_t DUTY_CYCLE = 0; // DUTY_CYCLE here is a value between 0 and 100 for calculation reasons
  uint16_t TOP = 0; // TOP value can be 16 bits as TIMER1 is 16 bits
  uint16_t PRESCALER = 0; // PRESCALER value
  uint32_t CRYSTAL_FREQUENCY = 16000000; // Crystal frequency is 16 MHz, written in Hz for simplicity
  
  switch (pin) {
    case PD5: // OC0B
      PRESCALER = calculatePrescaler(frequency, 0);
      setPrescaler(PRESCALER, 0);
      TOP = (uint8_t) ((CRYSTAL_FREQUENCY) / (frequency * PRESCALER)) - 1;
      DUTY_CYCLE = (TOP * value) / 100;
      OCR0A = TOP;
      OCR0B = DUTY_CYCLE;
      break;

    case PB2: // OC1B
      PRESCALER = calculatePrescaler(frequency, 1);
      setPrescaler(PRESCALER, 1);
      TOP = (uint16_t) ((CRYSTAL_FREQUENCY) / (frequency * PRESCALER)) - 1;
      DUTY_CYCLE = (TOP * value) / 100;
      OCR1A = TOP;
      OCR1B = DUTY_CYCLE;
      break;

    case PD3: // OC2B
      PRESCALER = calculatePrescaler(frequency, 2);
      setPrescaler(PRESCALER, 2);
      TOP = (uint8_t) ((CRYSTAL_FREQUENCY) / (frequency * PRESCALER)) - 1;
      DUTY_CYCLE = (TOP * value) / 100;
      OCR2A = TOP;
      OCR2B = DUTY_CYCLE;
      break;
  }
}

void setPrescaler(uint16_t PRESCALER_VALUE, uint8_t TIMER)
{
  if (TIMER == 0)
  {
    switch (PRESCALER_VALUE) {
      case 8:
        cbi(TCCR0B, CS00);
        sbi(TCCR0B, CS01);
        cbi(TCCR0B, CS02);
        break;
      case 64:
        sbi(TCCR0B, CS00);
        sbi(TCCR0B, CS01);
        cbi(TCCR0B, CS02);
        break;
      case 256:
        cbi(TCCR0B, CS00);
        cbi(TCCR0B, CS01);
        sbi(TCCR0B, CS02);
        break;
      case 1024:
        sbi(TCCR0B, CS00);
        cbi(TCCR0B, CS01);
        sbi(TCCR0B, CS02);
        break;
    }
  }

  if (TIMER == 1)
  {
     switch (PRESCALER_VALUE) {
      case 8:
        cbi(TCCR1B, CS10);
        sbi(TCCR1B, CS11);
        cbi(TCCR1B, CS12);
        break;
      case 64:
        sbi(TCCR1B, CS10);
        sbi(TCCR1B, CS11);
        cbi(TCCR1B, CS12);
        break;
      case 256:
        cbi(TCCR1B, CS10);
        cbi(TCCR1B, CS11);
        sbi(TCCR1B, CS12);
        break;
      case 1024:
        sbi(TCCR1B, CS10);
        cbi(TCCR1B, CS11);
        sbi(TCCR1B, CS12);
        break;
    }
  }

  if (TIMER == 2)
  {
     switch (PRESCALER_VALUE) {
      case 8:
        cbi(TCCR2B, CS20);
        sbi(TCCR2B, CS21);
        cbi(TCCR2B, CS22);
        break;
      case 64:
        cbi(TCCR2B, CS20);
        cbi(TCCR2B, CS21);
        sbi(TCCR2B, CS22);
        break;
      case 128:
        sbi(TCCR2B, CS20);
        cbi(TCCR2B, CS21);
        sbi(TCCR2B, CS22);
        break;
      case 256:
        cbi(TCCR2B, CS20);
        sbi(TCCR2B, CS21);
        sbi(TCCR2B, CS22);
        break;
      case 1024:
        sbi(TCCR2B, CS20);
        sbi(TCCR2B, CS21);
        sbi(TCCR2B, CS22);
        break;
    }
  }
}

uint16_t calculatePrescaler(uint32_t frequency, uint8_t TIMER) 
{
  // TIMER 0 HAS 8 / 32 / 64 / 256 / 1024 AS PRESCALER OPTIONS
  if (TIMER == 0) {
    if (frequency <= 1000000 && frequency > 125000) {
      return 8;
    }

    if (frequency <= 125000 && frequency > 31250) {
      return 64;
    }

    if (frequency <= 31250 && frequency > 7812) {
      return 256;
    }

    if (frequency <= 7812 && frequency > 62) {
      return 1024;
    }
  }

  // TIMER 1 HAS 8 / 64 / 256 / 1024 AS PRESCALER OPTIONS
  if (TIMER == 1) {
    if (frequency <= 1000000 && frequency > 125000) {
      return 8;
    }

    if (frequency <= 125000 && frequency > 62500) {
      return 64;
    }

    if (frequency <= 62500 && frequency > 31250) {
      return 128;
    }

    if (frequency <= 31250 && frequency > 7812) {
      return 256;
    }

    if (frequency <= 7812 && frequency > 0) {
      return 1024;
    }
  }

  // TIMER 2 HAS 8 / 64 / 128 / 256 / 1024 AS PRESCALER OPTIONS
  if (TIMER == 2) {
    if (frequency <= 1000000 && frequency > 125000) {
      return 8;
    }

    if (frequency <= 125000 && frequency > 62500) {
      return 64;
    }

    if (frequency <= 62500 && frequency > 31250) {
      return 128;
    }

    if (frequency <= 31250 && frequency > 7812) {
      return 256;
    }

    if (frequency <= 7812 && frequency > 62) {
      return 1024;
    }
  }
}

void timer0_init()
{
  // Clears TCCR0A and TCCR0B
  TCCR0A = 0;
  TCCR0B = 0;

  // WGM2:0 = 7 (TOP AT OCR0A)
  sbi(TCCR0A, WGM01);
  sbi(TCCR0A, WGM00);
  sbi(TCCR0B, WGM02);
  
  sbi(TCCR0A, COM0A0); // WGM02 = 1: Toggle OC0A on Compare Match.
  sbi(TCCR0A, COM0B1); // Clear OC0B on Compare Match, set OC0B at BOTTOM, (non-inverting mode)

  OCR0A = 0;
  OCR0B = 0;
}

void timer1_init()
{
  // Clears TCCR1A and TCCR1B
  TCCR1A = 0;
  TCCR1B = 0;

  // WGM13:0 = 15 (TOP AT OCR1A)
  sbi(TCCR1A, WGM10);
  sbi(TCCR1A, WGM11);
  sbi(TCCR1B, WGM12);
  sbi(TCCR1B, WGM13);

  // WGM13:0 = 14 or 15:  Toggle OC1A on Compare Match
  cbi(TCCR1A, COM1A1);
  sbi(TCCR1A, COM1A0);

  // Clear OC1B on Compare Match, set OC1B at BOTTOM (non-inverting mode)
  sbi(TCCR1A, COM1B1);
  cbi(TCCR1A, COM1B0);

  OCR1A = 0;
  OCR1B = 0;
}

void timer2_init()
{
  // Clears TCCR2A and TCCR2B
  TCCR2A = 0;
  TCCR2B = 0;

  // WGM22:0 = 7 (TOP AT OCR2A)
  sbi(TCCR2A, WGM21);
  sbi(TCCR2A, WGM20);
  sbi(TCCR2B, WGM22);

  
  sbi(TCCR2A, COM2A0); // Toggle OC2A on Compare Match
  sbi(TCCR2A, COM2B1); // Clear OC2A on Compare Match, set OC2A at BOTTOM, (non-inverting mode)

  OCR2A = 0;
  OCR2B = 0;
}

void setup() {
  cli(); // Stop interrupts

  adc_init(); // Turns on ADC
  timer0_init(); // Turns on TIMER0
  timer1_init(); // Turns on TIMER1
  timer2_init(); // Turns on TIMER2

  cbi(DDRC, POTENTIOMETER_PIN); // Potentiometer as INPUT

  // Green and Blue LEDs as OUTPUT
  sbi(DDRD, LED_GREEN_PIN);
  sbi(DDRD, LED_BLUE_PIN);

  cbi(DDRD, BUTTON_PIN); // Button as INPUT

  // RGB LEDs as OUTPUT
  sbi(DDRD, PWM_RED_PIN);
  sbi(DDRD, PWM_GREEN_PIN);
  sbi(DDRB, PWM_BLUE_PIN);

  sei(); // Allow interrupts
}

void loop() {
  uint8_t POTENTIOMETER_VALUE = adc_read(POTENTIOMETER_PIN);
  uint8_t POTENTIOMETER_PERCENT = (POTENTIOMETER_VALUE - 0) * (100 - 0) / (255 - 0) + 0;
  bool BUTTON_ACTIVE = bit_is_set(PINB, BUTTON_PIN);

  if (BUTTON_ACTIVE && !filter) {
    filter = 1;
    switch (count) {
      case 0:
        SELECTED = PWM_RED_PIN;
        break;
      case 1:
        SELECTED = PWM_GREEN_PIN;
        break;
      case 2:
        SELECTED = PWM_BLUE_PIN;
        break;
      default:
        SELECTED = 0;
        break;
    }
    count += 1;
  } else if (!BUTTON_ACTIVE) {
    filter = 0;
  }
 
  // Se o valor do potenciometro é de 255 (100%), liga o led azul, se for 0, liga o verde, se nenhum, desliga os 2, margem de erro de 10
  if (POTENTIOMETER_VALUE >= 250) {
    cbi(PORTD, LED_GREEN_PIN);
    sbi(PORTD, LED_BLUE_PIN);
  } else if (POTENTIOMETER_VALUE <= 10) {
    sbi(PORTD, LED_GREEN_PIN);
    cbi(PORTD, LED_BLUE_PIN);
  } else {
    cbi(PORTD, LED_BLUE_PIN);
    cbi(PORTD, LED_GREEN_PIN);
  }

  adc_write(SELECTED, POTENTIOMETER_PERCENT, 200);
}
