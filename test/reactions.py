import unittest
import cmf

t = cmf.Time()
def setup():
    p = cmf.project('A B C')
    S = p.NewStorage('S')
    return p, S

class ReactionObjects(unittest.TestCase):
    def test_Reaction_01_access(self):
        """Test if the reactions behave like a list"""
        p, S = setup()
        A = p.solutes[0]
        self.assertEqual(len(S[A].reactions), 0)

    def test_Reaction_02_constant(self):
        """Test a cmf.SoluteConstantFluxReaction"""
        p, S = setup()
        A = p.solutes[0]
        reaction = cmf.SoluteConstantFluxReaction(1.0)
        S[A].reactions = [reaction]
        self.assertEqual(S[A].dxdt(t), 1.0, 'Change rate of solute does not equal reaction')
        reaction.flux = 2.0
        self.assertEqual(S[A].dxdt(t), 2.0, 'Change rate of solute does not react to parameter change of parent')

    def test_Reaction_03_decay(self):
        """Test cmf.SoluteDecayReaction"""
        p, S = setup()
        A = p.solutes[0]
        reaction = cmf.SoluteDecayReaction(1.0)
        S[A].state = 1.0
        S[A].reactions = [reaction]
        self.assertEqual(S[A].dxdt(t), -1.0, 'Change rate of solute does not equal reaction')
        S[A].state = 0.5
        self.assertEqual(S[A].dxdt(t), -0.5, 'Change rate of solute does not react to state change')
        reaction.decay = 2.0
        self.assertEqual(S[A].dxdt(t), -1.0, 'Change rate of solute does not react to parameter change of parent')

    def test_Reaction_04_Equilibrium(self):
        """Test cmf.SoluteEquilibriumReaction"""
        p, S = setup()
        A, B = list(p.solutes)[:2]
        reaction = cmf.SoluteEquilibriumReaction(A, B, 1.0, 1.0)
        S.volume = 1.0
        S[A].state = 1.0
        S[B].state = 0.0
        S[B].reactions = S[A].reactions = [reaction]

        self.assertEqual(S[A].dxdt(t), -1.0, 'dA/dt: Change rate of solute does not equal reaction')
        self.assertEqual(S[B].dxdt(t), 1.0, 'dB/dt: Change rate of solute does not equal other Solute')
        S[B].state = 1.0
        self.assertEqual(S[A].dxdt(t), 0.0, 'Change rate of solute not zero for equilibrium')

    def test_Reaction_05_1st_order(self):
        """Test cmf.Solute1stOrderReaction"""
        p, S = setup()
        A, B = p.solutes[:2]
        reaction = cmf.Solute1stOrderReaction(A, B, 1.0)
        S.volume = 1.0
        S[A].state = 1.0
        S[B].state = 0.0
        S[B].reactions = S[A].reactions = [reaction]

        self.assertEqual(S[A].dxdt(t), -1.0, 'dA/dt: Change rate of solute does not equal reaction')
        self.assertEqual(S[B].dxdt(t), 1.0, 'dB/dt: Change rate of solute does not equal other Solute')
        S[B].state = 1.0
        self.assertEqual(S[A].dxdt(t), -1.0, 'Change rate of solute is changed by concentration of product')

    def test_Reaction_06_2nd_order(self):
        """Test cmf.Solute1stOrderReaction"""
        p, S = setup()
        A, B, C = p.solutes[:]
        reaction = cmf.Solute2ndOrderReaction(A, B, C, 1.0)
        S.volume = 1.0
        S[A].state = 0.5
        S[B].state = 2.0
        for X in p.solutes:
            S[X].reactions = [reaction]

        self.assertEqual(S[A].dxdt(t), -1.0, 'dA/dt: Change rate of solute does not equal reaction')
        self.assertEqual(S[B].dxdt(t), -1.0, 'dB/dt: Change rate of solute does not equal other Solute')
        self.assertEqual(S[C].dxdt(t), 1.0, 'Change rate of solute is changed by concentration of product')

    def test_Reaction_10_director(self):
        """Test cmf.SoluteReaction in a Python implementation"""
        p, S = setup()
        A = p.solutes[0]

        class R(cmf.SoluteReaction):
            def get_flux(self, solstor, t):
                return 1.0
        r = R()
        S[A].reactions = [r]
        self.assertEqual(S[A].dxdt(t), 1.0, 'Change rate of solute does not equal reaction')


if __name__ == '__main__':
    unittest.main(verbosity=2)
