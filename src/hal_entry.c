/***********************************************************************************************************************
 * File Name    : hal_entry.c
 * Description  : Contains data structures and functions used in hal_entry.c.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

#include "common_utils.h"
#include "icu_ep.h"


/*******************************************************************************************************************//**
 * @addtogroup icu_ep
 * @{
 **********************************************************************************************************************/


FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER


volatile bool b_ready_to_read = false;

static uint16_t g_adc_data,g_adc_data1;
/* Board's user LED */
extern bsp_leds_t g_bsp_leds;
/* Boolean flag to determine switch is pressed or not.*/
extern volatile bool g_sw_press;
static fsp_err_t adc_scan_start(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    if (false == b_ready_to_read)
    {
        /* Open/Initialize ADC module */
        err = R_ADC_Open (&g_adc_ctrl, &g_adc_cfg);

        /* handle error */
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
          //  APP_ERR_PRINT("** R_ADC_Open API failed ** \r\n");
            return err;
        }
        /* Configures the ADC scan parameters */
        err = R_ADC_ScanCfg (&g_adc_ctrl, &g_adc_channel_cfg);
        /* handle error */
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
           // APP_ERR_PRINT("** R_ADC_ScanCfg API failed ** \r\n");
            return err;
        }

        /* Start the ADC scan*/
        err = R_ADC_ScanStart (&g_adc_ctrl);

        /* handle error */
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
         //   APP_ERR_PRINT("** R_ADC_ScanStart API failed ** \r\n");
            return err;
        }

        //APP_PRINT("\r\nADC Started Scan\r\n");
        //uart_print_user_msg("\r\nADC Started Scan\r\n");
        /* Indication to start reading the adc data */
        b_ready_to_read = true;
    }
    else
    {
      //  APP_PRINT("\r\nADC Scan already in progress\r\n");
       // uart_print_user_msg("\r\nADC Scan already in progress\r\n");
    }

    return err;
}
fsp_err_t adc_read_data(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status
    //uint8_t msg_len = RESET_VALUE;
   // char *p_temp_ptr = (char *)ADC_msg;
    //msg_len = ((uint8_t)(strlen(p_temp_ptr)));
    /* Read the result */
    err = R_ADC_Read (&g_adc_ctrl,ADC_CHANNEL_6, &g_adc_data);

    /* handle error */
    if (FSP_SUCCESS != err)
    {
        /* ADC Failure message */
        //APP_ERR_PRINT("** R_ADC_Read API failed ** \r\n");
      //  uart_print_user_msg("** R_ADC_Read API failed ** \r\n");
        return err;
    }
    /*
    ADC_msg[msg_len-6]=(g_adc_data/1000)+0x30;
    ADC_msg[msg_len-5]=(g_adc_data/100)%10+0x30;
    ADC_msg[msg_len-4]=(g_adc_data/10)%100%10+0x30;
    ADC_msg[msg_len-3]=(g_adc_data%10)+0x30;
    uart_print_user_msg(&ADC_msg);*/
   // APP_PRINT("\r\nMCU Die Temperature Reading from ADC: %d\r\n", g_adc_data);
   // uart_print_user_msg
         //      printf("\r\nMCU Die Temperature Reading from ADC: %d\r\n", g_adc_data);

    //uart_print_user_msg("\r\nMCU Die Temperature Reading from ADC:");


                //  uart_print_user_msg("\r\nMCU Die Temperature Reading from ADC:");
                 // uart_print_user_msg("\r\n");
    //void DbgUartTrace(const char *lpszFormat, ...)
    //DbgUartTrace("\r\nMCU Die Temperature Reading from ADC: %d\r\n", g_adc_data);
    /* In adc single scan mode after reading the data, it stops.So reset the b_ready_to_read state to
     * avoid reading unnecessarily. close the adc module as it gets opened in start scan command.*/
    if (ADC_MODE_SINGLE_SCAN == g_adc_cfg.mode)
    {
        b_ready_to_read = false;

        /* Close the ADC module*/
        err = R_ADC_Close (&g_adc_ctrl);

        /* handle error */
        if (FSP_SUCCESS != err)
        {
            /* ADC Failure message */
           // APP_ERR_PRINT("** R_ADC_Close API failed ** \r\n");
            return err;
        }

       // APP_PRINT("\r\nPress any other key(except 1 and 2) to go back to the main menu\r\n");
    }
    /*
#ifdef BOARD_RA2A1_EK
    // check for deviation in adc values to initiate the calibration again//
    err = adc_deviation_in_output ();
    // handle error //
    if (FSP_SUCCESS != err)
    {
        // ADC Failure message //
        APP_ERR_PRINT("** adc_deviation_in_output function failed ** \r\n");
        return err;
    }

    // update the current adc data to previous //
    g_prev_adc_data = g_adc_data;

#endif
*/
    /* 1 Seconds Wait time between successive readings */
   // R_BSP_SoftwareDelay (ADC_READ_DELAY, BSP_DELAY_UNITS_SECONDS);
    return err;
}
/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void ADC_Task(void)
{
    fsp_err_t err                           = FSP_SUCCESS;
    R_BSP_SoftwareDelay(250, BSP_DELAY_UNITS_MICROSECONDS);
      err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_HIGH);
      err = adc_scan_start();
      err = adc_read_data();
      err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_LOW);
      if (FSP_SUCCESS != err)
                 {
                  //   APP_ERR_PRINT("** R_IOPORT_PinRead FAILED ** \r\n");
                     /* Close External IRQ module.*/
                  //   icu_deinit();
                   //  APP_ERR_TRAP(err);
                 }
}
void hal_entry(void)
{
    fsp_err_t err                           = FSP_SUCCESS;
    bsp_io_level_t led_current_state        = (bsp_io_level_t) RESET_VALUE;
    fsp_pack_version_t version              = {RESET_VALUE};

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

    /* Example Project information printed on the RTT */
    APP_PRINT(BANNER_INFO, EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);
    APP_PRINT(EP_INFO);

    /* Initialize External IRQ driver*/
    err = icu_init();
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** ICU INIT FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }

    /* Enable External IRQ driver*/
    err = icu_enable();
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        APP_ERR_PRINT("** ICU ENABLE FAILED ** \r\n");
        /* Close External IRQ module.*/
        icu_deinit();
        APP_ERR_TRAP(err);
    }

    /* If this board has no LEDs then trap here */
    if (RESET_VALUE == leds.led_count)
    {
        APP_PRINT("\r\nThere are no LEDs on this board\r\n");
        /* Close External IRQ module.*/
        icu_deinit();
        APP_ERR_TRAP(err);
    }
    err = R_GPT_Open(&g_timer_pwm_ctrl, &g_timer_pwm_cfg);
        err = R_GPT_Start(&g_timer_pwm_ctrl);
    /* Main loop */
    while (true)
    {
        /* Toggle user LED  when user pushbutton is pressed*/
        if(true == g_sw_press)
        {
            /* Clear user pushbutton flag */
            g_sw_press = false;

            /* Notify that user pushbutton is pressed */
           // APP_PRINT("\r\nUser Pushbutton Pressed\r\n");
           // R_BSP_PinWrite(BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_HIGH);
            /* Read user LED  pin */
          //  err = R_IOPORT_PinRead(&g_ioport_ctrl, (bsp_io_port_pin_t)leds.p_leds[RESET_VALUE], &led_current_state);
  /*
            err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_HIGH);
            err = adc_scan_start();
            err = adc_read_data();
            err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_LOW);
*/
            /* Handle error */
            if (FSP_SUCCESS != err)
            {
             //   APP_ERR_PRINT("** R_IOPORT_PinRead FAILED ** \r\n");
                /* Close External IRQ module.*/
             //   icu_deinit();
              //  APP_ERR_TRAP(err);
            }
         //   R_BSP_PinWrite(BSP_IO_PORT_02_PIN_05, BSP_IO_LEVEL_LOW);
            /* Reverse LED pin state*/
           // led_current_state ^= BSP_IO_LEVEL_HIGH;

            /* Toggle user LED */
           // err = R_IOPORT_PinWrite(&g_ioport_ctrl, (bsp_io_port_pin_t)leds.p_leds[RESET_VALUE], led_current_state);

            /* Handle error */
            if (FSP_SUCCESS != err)
            {
                //APP_ERR_PRINT("** R_IOPORT_PinWrite FAILED ** \r\n");
                /* Close External IRQ module.*/
             //   icu_deinit();
             //   APP_ERR_TRAP(err);
            }

#if defined (BOARD_RA6T1_RSSK) || defined (BOARD_RA4W1_EK)
            if(BSP_IO_LEVEL_LOW == led_current_state)
#else
                if(BSP_IO_LEVEL_HIGH == led_current_state)
#endif
                {
                    /* Print LED Pin state */
                   // APP_PRINT("LED State: High{ON}\r\n");
                }
                else
                {
                    /* Print LED Pin state */
                  //  APP_PRINT("LED State: Low{OFF}\r\n");
                }
        }
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event) {
    if (BSP_WARM_START_RESET == event) {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event) {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}

/*******************************************************************************************************************//**
 * @} (end addtogroup icu_ep)
 **********************************************************************************************************************/
