class State {
public:
    virtual ~State() = default;
    
    virtual State* handle(Event e) = 0; 
    
    virtual void execute() = 0;
};