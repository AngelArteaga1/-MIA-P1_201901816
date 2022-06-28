import { ComponentFixture, TestBed } from '@angular/core/testing';

import { RecoverConfirmComponent } from './recover-confirm.component';

describe('RecoverConfirmComponent', () => {
  let component: RecoverConfirmComponent;
  let fixture: ComponentFixture<RecoverConfirmComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ RecoverConfirmComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(RecoverConfirmComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
