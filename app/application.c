#include <application.h>
#include <bc_servo.h>

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

bc_servo_t servo;

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    static uint8_t angle = 0;

    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_set_mode(&led, BC_LED_MODE_TOGGLE);

        bc_log_info("bc_servo_set_angle %d", angle);

        bc_servo_set_angle(&servo, angle);

        angle += 30;

        if (angle > 180)
        {
            angle = 0;
        }
    }
}

void application_init(void)
{
    bc_log_init(BC_LOG_LEVEL_DEBUG, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_servo_init(&servo, BC_PWM_P0);
    bc_servo_set_microseconds(&servo, 1500);

    bc_log_info("bc_servo_set_microseconds 1500us");
}
