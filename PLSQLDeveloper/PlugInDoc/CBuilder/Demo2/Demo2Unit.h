//---------------------------------------------------------------------------
#ifndef Demo2UnitH
#define Demo2UnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TDemoForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *TopPanel;
    TBitBtn *ExecuteBtn;
    TBitBtn *CloseBtn;
    TMemo *Memo;
    TStringGrid *Grid;
    void __fastcall ExecuteBtnClick(TObject *Sender);
    void __fastcall CloseBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TDemoForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDemoForm *DemoForm;
//---------------------------------------------------------------------------
#endif
