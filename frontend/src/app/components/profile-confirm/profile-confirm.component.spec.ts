import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProfileConfirmComponent } from './profile-confirm.component';

describe('ProfileConfirmComponent', () => {
  let component: ProfileConfirmComponent;
  let fixture: ComponentFixture<ProfileConfirmComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ProfileConfirmComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ProfileConfirmComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
