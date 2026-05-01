class SensorObserver {
    public:
        Event notify();
        void set_observer(SensorObserver*);
}