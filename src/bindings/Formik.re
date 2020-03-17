/**To bind to formik we'll need to most likely use Js.Dict.t type to accomodate the variable property fields
 * https://github.com/jaredpalmer/formik/blob/master/packages/formik/src/types.tsx
 * Functors might also be an option: https://2ality.com/2018/01/functors-reasonml.html
 */

type values('v) = 'v;
type errors('e) = 'e;
type touched('t) = 't;

type state('v, 'e, 't, 's) = {
  values: values('v),
  errors: errors('e),
  touched: touched('t),
  isSubmitting: bool,
  isValidating: bool,
  status: 's,
  submitCount: int,
};

type setFormikState('v, 'e, 't, 's) = state('v, 'e, 't, 's) => 'v;
type callback = unit => unit;

type helpers('v, 'e, 't, 's, 'value) = {
  setStatus: 's => unit,
  setErrors: 'e => unit,
  setSubmitting: bool => unit,
  /**bool = shouldValidate */
  setTouched: ('t, bool) => unit,
  /**bool = shouldValidate */
  setValues: ('v, bool) => unit,
  /**(fieldName, shouldValidate) */
  setFieldValue: (string, 'value, bool) => unit,
  /**(fieldName, message) */
  setFieldError: (string, string) => unit,
  /**(fieldName, isTouched, shouldValidate) */
  setFieldTouched: (string, bool, bool) => unit,
  validateForm: unit => Js.Promise.t('v),
  /** (fieldName) */
  validateField: string => unit,
  resetForm: unit => unit,
  submitForm: unit => Js.Promise.t(unit),
  setFormikState: (setFormikState('v, 'e, 't, 's), callback) => unit,
};

type fieldInputProps('value) = {
  value: 'value,
  name: string,
  multiple: bool,
  checked: bool,
  onChange: ReactEvent.Form.t,
  onBlur: ReactEvent.Form.t,
};

type fieldMetaProps('value) = {
  value: 'value,
  error: string,
  touched: bool,
  initialValue: 'value,
  initialTouched: bool,
  initialError: string,
};

type fieldHelperProps('value) = {
  setValue: 'value => unit,
  setTouched: bool => unit,
  setError: 'value => unit,
};

type validator('a) = 'a => unit;

type registerFieldFns('a) = {validate: validator('a)};

type props('v, 'e, 't, 's, 'value, 'registerFieldFns) = {
  /** START: Shared Config */
  validateOnChange: bool,
  validateOnBlur: bool,
  validateOnMount: bool,
  isInitialValid: bool,
  enableReinitialize: bool,
  /** START: State */
  values: values('v),
  errors: errors('e),
  touched: touched('t),
  isSubmitting: bool,
  isValidating: bool,
  status: 's,
  submitCount: int,
  /** START: Helpers */
  setStatus: 's => unit,
  setErrors: 'e => unit,
  setSubmitting: bool => unit,
  /**bool = shouldValidate */
  setTouched: ('t, bool) => unit,
  /**bool = shouldValidate */
  setValues: ('v, bool) => unit,
  /**(fieldName, shouldValidate) */
  setFieldValue: (string, 'value, bool) => unit,
  /**(fieldName, message) */
  setFieldError: (string, string) => unit,
  /**(fieldName, isTouched, shouldValidate) */
  setFieldTouched: (string, bool, bool) => unit,
  validateForm: unit => Js.Promise.t('v),
  /** (fieldName) */
  validateField: string => unit,
  resetForm: unit => unit,
  submitForm: unit => Js.Promise.t(unit),
  setFormikState: (setFormikState('v, 'e, 't, 's), callback) => unit,
  /** START: Handlers */
  handleSubmit: ReactEvent.Form.t => unit,
  handleReset: ReactEvent.Synthetic.t => unit,
  handleBlur: ReactEvent.Focus.t,
  handleChange: ReactEvent.Form.t,
  /** string was (props: any) in ts typings */
  getFieldProps: string => fieldInputProps('value),
  getFieldMeta: string => fieldMetaProps('value),
  getFieldHelpers: string => fieldInputProps('value),
  /** START: computedProps */
  dirty: bool,
  isValid: bool,
  initialValues: values('v),
  initialErrors: errors('e),
  initialTouched: touched('t),
  initialStatus: 's,
  /** START : registration *//**(string: fieldName) */
  registerField: (string, registerFieldFns('registerFieldFns)) => unit,
  /**(string: fieldName) */
  unregisterField: string => unit,
};

type config('v, 'e, 't, 's, 'value, 'validation, 'innerRef) = {
  /** START: Shared Config */
  validateOnChange: bool,
  validateOnBlur: bool,
  validateOnMount: bool,
  isInitialValid: bool,
  enableReinitialize: bool,
  /** Rest... */
  render: 'v => React.element,
  children: React.element,
  initialValues: 'v,
  initialStatus: 's,
  initialErrors: errors('e),
  initialTouched: touched('e),
  onReset: ('v, helpers('v, 'e, 't, 's, 'value)) => unit,
  onSubmit: ('v, helpers('v, 'e, 't, 's, 'value)) => unit,
  validationSchema: 'validation,
  validate: 'v => Js.Nullable.t(Js.Promise.t(errors('v))),
  innerRef: React.Ref.t('innerRef) => unit,
};

type arrayHelpers('a) = {
  push: 'a => unit,
  remove: int => unit,
};

type onSubmit('v, 'e, 't, 's, 'value) =
  (values('v), helpers('v, 'e, 't, 's, 'value)) => unit;

type useFormikParams('v, 'e, 't, 's, 'value) = {
  initialValues: 'v,
  onSubmit: onSubmit('v, 'e, 't, 's, 'value),
};

[@bs.module "formik"]
external useFormik:
  useFormikParams('v, 'e, 't, 's, 'value) =>
  props('v, 'e, 't, 's, 'value, 'registerFieldFns) =
  "useFormik";

module FieldArray = {
  [@bs.module "formik"] [@react.component]
  external make:
    (~name: string, ~render: arrayHelpers('a) => React.element) =>
    React.element =
    "FieldArray";
};

module Form = {
  [@bs.module "formik"] [@react.component]
  external make: (~children: React.element) => React.element = "Form";
};

type renderProps('v, 'e, 't, 's, 'value, 'registerFieldFns) =
  props('v, 'e, 't, 's, 'value, 'registerFieldFns) => React.element;

type validationSchema;

[@bs.module "formik"] [@react.component]
external make:
  (
    ~initialValues: 'v,
    ~validationSchema: validationSchema=?,
    ~enableReinitialize: bool=?,
    ~validateOnBlur: bool=?,
    ~validateOnChange: bool=?,
    ~validateOnMount: bool=?,
    ~onSubmit: onSubmit('v, 'e, 't, 's, 'value),
    ~validate: 'v => unit=?,
    ~render: renderProps('v, 'e, 't, 's, 'value, 'registerFieldFns)=?,
    ~children: React.element=?
  ) =>
  React.element =
  "Formik";