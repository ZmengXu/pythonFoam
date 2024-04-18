import numpy as np
from scipy.integrate import odeint
def bubble_growth(y, t, R0, CL, Pv, Pr, Pr0, sigma, n, muL, kappa, rho):
     R, R_dot = y
     R_dot_dot = ((1 + R_dot/CL)*(Pv + (Pr0+2*sigma/R0)*((R0/R)**(3*n)) -2*sigma/R - 4*muL*R_dot/R - 4*kappa*R_dot/(R**2) - Pr)/rho + R_dot*(-3*(Pr0 + 2*sigma/R0)*(R0**(3*n))*n*R_dot/(R**(3*n+1)) + 2*sigma*R_dot/(R**2) + 4*muL*(R_dot**2)/(R**2) + 8*kappa*(R_dot**2)/(R**3))/(rho*CL) - (3/2 - R_dot/(2*CL))*(R_dot**2)) / ((1-R_dot/CL)*R - R_dot*(-4*muL/R - 4*kappa/(R**2))/(rho*CL))

     return [R_dot, R_dot_dot]

def solve(initial_conditions, R0, CL, Pv, Pr, Pr0, sigma, n, muL, kappa, rho, t_span, specific_time):
     R0, R_dot0 = initial_conditions
     t = np.linspace(*t_span, num=100)
     solution = odeint(bubble_growth, [R0, R_dot0], t, args=(R0, CL, Pv, Pr, Pr0, sigma, n, muL, kappa, rho))
     R_values = solution[:, 0]

     idx = (np.abs(t - specific_time)).argmin()
     return R_values[idx]

