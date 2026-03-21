class vehicleDynamics{
    public:
    constexpr float estimateNormalForce_N();
    constexpr float estimateBodySlip(); 
    constexpr float estimateVehicleSpeed_MPS();
    struct {
        float v_x; 
        float v_y;
        float v_z; 
    } velocity_t; 
    
    velocity_t vehicle_v
    private:
    constexpr float estimateLoadTransfer_N();
    constexpr float estimateDownForce_N();


    /* What does this class need to do? 
        - estimate load transfer
        - estimate down force 
        - estimate normal force -- needs to be public 
        - estimate body slip -- needs to be public
        - estimate vehicle speed -- needs to be public 
    */
};

/* Leaving velocity estimate for EKF */

 

