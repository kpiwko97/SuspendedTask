

class TimeHelper
{
private:
    static unsigned long now;

    static unsigned long TimeDifference() {
        return millis() - now;
    }
public:
    static bool Delay(uint32_t waitTime)
    {
        unsigned long test = 0;
        now = millis();

        while (TimeDifference() <= waitTime - 1)
        {
            //Serial.println(micros());
            if (TimeDifference() > test)
            {
                Serial.println("Long task");
                test += 500;
            }
        }
    }
};

unsigned long TimeHelper::now = 0;

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;
TaskHandle_t TaskHandle_4;

void setup()
{

    Serial.begin(115200);
    Serial.println(F("In Setup function"));
}


void loop()
{
    xTaskCreatePinnedToCore(MyTask1, "Task1", 1000, NULL, 1, &TaskHandle_1,0);
    xTaskCreatePinnedToCore(MyTask3, "Task3", 1200, NULL, 3, &TaskHandle_3,0);
    // Hooked to IDle task, it will run whenever CPU is idle
    Serial.println(F("Loop function"));
    Serial.println();
    delay(5000);


}


/* Task1 with priority 1 */
static void MyTask1(void* pvParameters)
{
    while (1)
    {
        Serial.println(F("Task1 Running and About to delete itself"));
        vTaskDelete(TaskHandle_1);    // Delete the task using the TaskHandle_1
    }
}


/* Task2 with priority 2 */
static void MyTask2(void* pvParameters)
{
    while (1)
    {
        Serial.println(F("Task2 Running and About to delete itsel"));
        TimeHelper::Delay(3000);
        vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
    }
}


/* Task3 with priority 3 */
static void MyTask3(void* pvParameters)
{
    while (1)
    {
        Serial.println(F("Task3 Running, Creating Task2 and Task4"));
        xTaskCreatePinnedToCore(MyTask2, "Task2", 1000, NULL, 2, &TaskHandle_2,0);
        xTaskCreatePinnedToCore(MyTask4, "Task4", 1200, NULL, 4, &TaskHandle_4,0); 

        TimeHelper::Delay(1000);
        
        Serial.println(F("Task 3 Suspend Task 2"));
        vTaskSuspend(TaskHandle_2);

        TimeHelper::Delay(3000);
        
        Serial.println(F("Back in Task3 and About to delete itself"));
        vTaskDelete(TaskHandle_3);
    }
}


/* Task4 with priority 4 */
static void MyTask4(void* pvParameters)
{
    while (1)
    {
        Serial.println(F("Task4 Running and About to delete itself"));
        vTaskDelete(NULL);     //Delete own task by passing NULL(TaskHandle_2 can also be used)
    }
}
