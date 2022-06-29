import { ComponentFixture, TestBed } from '@angular/core/testing';

import { WithoutAccessComponent } from './without-access.component';

describe('WithoutAccessComponent', () => {
  let component: WithoutAccessComponent;
  let fixture: ComponentFixture<WithoutAccessComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ WithoutAccessComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(WithoutAccessComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
