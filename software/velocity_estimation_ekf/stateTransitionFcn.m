function xk = stateTransitionFcn(x, w, u)
    % xkm1: [Vx; Vy] at previous step
    % u:    vector of inputs [ax; ay; yawrate; T] (or however you prefer)

    % Extract states and inputs
    Vx_prev    = x(1);
    Vy_prev    = x(2);
    ax         = u(1);
    ay         = u(2);
    r          = u(3);
    T          = u(4);

    % State transition
    Vx_k = Vx_prev + T*(ax + Vy_prev * r);
    Vy_k = Vy_prev + T*(ay + Vx_prev * r);

    % Return next state
    xk = [Vx_k; Vy_k];
end