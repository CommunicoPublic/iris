/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "DataBuffer.hpp"
#include "LineBuffer.hpp"

#include <stdio.h>
#include <sysexits.h>

using namespace IRIS;

int main(void)
{
	LineBuffer  oBuffer(20);
	oBuffer.AddData("Non habebis deos alienos in conspectu meo. Non facies tibi sculptile nec similitudinem omnium, quae in caelo sunt ");
	oBuffer.AddData("desuper et quae in terra deorsum et quae versantur in aquis sub terra. Non adorabis ea et non coles: Ego enim sum Dominus Deus tuus, ");
	oBuffer.AddData("Deus aemulator, reddens iniquitatem patrum super filios in tertiam et quartam generationem his, qui oderunt me, et faciens misericordiam ");
	oBuffer.AddData("in multa milia diligentibus me et custodientibus praecepta mea.\n");
	oBuffer.AddData("Non usurpabis nomen Domini Dei tui frustra, quia non erit impunitus, qui super re vana nomen eius assumpserit.\n");
	oBuffer.AddData("Observa diem sabbati, ut sanctifices eum, sicut praecepit tibi Dominus Deus tuus. Sex diebus operaberis et facies omnia opera tua. Septimus dies sabbatum est Domino Deo tuo. Non facies in eo quidquam operis tu et filius tuus et filia, servus et ancilla et bos et asinus et omne iumentum tuum et peregrinus tuus, qui est intra portas tuas, ut requiescat servus tuus et ancilla tua sicut et tu. Memento quod et ipse servieris in Aegypto, et eduxerit te inde Dominus Deus tuus in manu forti et brachio extento: idcirco praecepit tibi, ut observares diem sabbati.");
	oBuffer.AddData("Honora patrem tuum et matrem, sicut praecepit tibi Dominus Deus tuus, ut longo vivas tempore et bene sit tibi in terra, quam Dominus Deus tuus daturus est tibi.");

	DataBuffer oData;

	fprintf(stderr, "1\n");
	while (oBuffer.GetLine(oData)) { fprintf(stderr, "%.*s\n", UINT_32(oData.Size()), oData.Data()); }

	oBuffer.AddData("Non occides.\nNeque moechaberis.\nFurtumque non facies.\nNec loqueris contra proximum tuum falsum testimonium.\nNec concupisces uxorem proximi tui.\n");

	fprintf(stderr, "2\n");
	oBuffer.Clear();
	while (oBuffer.GetLine(oData)) { fprintf(stderr, "%.*s\n", UINT_32(oData.Size()), oData.Data()); }

	oBuffer.AddData("Non occides.\nNeque moechaberis.\nFurtumque non facies.\nNec loqueris contra proximum tuum falsum testimonium.\nNec concupisces uxorem proximi tui.\n");
	oBuffer.AddData("Nec desiderabis domum proximi tui, non agrum, non servum, non ancillam, non bovem, non asinum et universa, quae illius sunt.");

	fprintf(stderr, "3\n");
	while (oBuffer.GetLine(oData)) { fprintf(stderr, "%.*s\n", UINT_32(oData.Size()), oData.Data()); }

	oBuffer.AddData("\nDixi.\n");

	fprintf(stderr, "4\n");
	while (oBuffer.GetLine(oData)) { fprintf(stderr, "%.*s\n", UINT_32(oData.Size()), oData.Data()); }

	oBuffer.Clear();

	fprintf(stderr, "5\n");
	while (oBuffer.GetLine(oData)) { fprintf(stderr, "%.*s\n", UINT_32(oData.Size()), oData.Data()); }

return EX_OK;
}
// End.
