function Fk = stateTransitionJacobianFcn(T, r)
    Fk = [1,         T*r; 
         -T*r, 1       ];
end